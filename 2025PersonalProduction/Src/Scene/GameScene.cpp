#include "Scene/GameScene.h"
#include <GSeffect.h>
#include "Assets.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"
#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "Item/ItemDataManager.h"
#include "Camera/FixedCamera.h"
#include "Camera/TimelineCamera.h"
#include "Camera/PlayerCamera.h"

// 一時的 ローダーやマネージャーを作ったら不要
#include <gslib.h>	// シーン終了用
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"	// タイムラインローダーを作るべし
#include "Actor/Player/Player.h"	// アクターローダーを作るべし
#include "Actor/Enemy/DummyEnemy.h"	// アクターローダーを作るべし
#include "Actor/ItemActor/ItemActor.h"  //アクターローダーを作るべし

#ifdef _DEBUG
#include "Camera/EditorCamera.h"
#endif

void GameScene::load() {
    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;

    load_data();

    // 終了
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void GameScene::start() {
	is_end_ = false;

    // TODO シーン終了時選べるようにしたり、ポーズで書き換えること
    next_scene_tag_ = SceneTag::Menu;

    game_start();
}

void GameScene::update(float delta_time) {
	// TODO 一時的なタイトルに戻る処理
	if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;

	world_.update(delta_time);

	// TODO 一時的なゲーム終了処理
	{
		if (world_.count_actor_with_tag(ActorTag::Enemy) <= 0) {
			is_end_ = true;
			return;
		}
		if (world_.count_actor_with_tag(ActorTag::Player) <= 0) {
			is_end_ = true;
			return;
		}
	}
}

void GameScene::draw() const {
	world_.draw();	
}

void GameScene::end() {
    game_end();
}

SceneTag GameScene::scene_tag() const {
	return SceneTag::Game;
}

bool GameScene::is_application_end() const {
	return false;
}

void GameScene::reception_message(const std::string& message, std::any& param) {
	// なにも受け取らない
}

void GameScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 8;
    const float progress = 1.0f / (float)count;

    // 共通アセットの読み込み
    if (AssetsManager::get_instance().find(AssetsLoader::GAME_COMMON_ASSET_NAME));
    else  AssetsLoader::load_by_json("Resource/Private/LoadAsset/game_common.json", AssetsLoader::GAME_COMMON_ASSET_NAME);
    load_progress_ += progress;

    // ステージ固有アセットの読み込み
    AssetsLoader::load_by_json("Resource/Private/LoadAsset/test_stage.json", AssetsLoader::GAME_STAGE_ASSET_NAME);
    load_progress_ += progress;

    // ライトマップの読み込み
    gsLoadLightmap(0, "Resource/Assets/Octree/Stage1/Lightmap/Lightmap.txt");
    load_progress_ += progress;
    // リフレクションプローブの読み込み
    gsLoadReflectionProbe(0, "Resource/Assets/Octree/Stage1/RefProbe/ReflectionProbe.txt");
    load_progress_ += progress;

    // ナビメッシュデータの読み込み
    world_.add_navmesh(new NavMeshSurface{ "Resource/Assets/Octree/Stage1/navmesh.txt" });
    load_progress_ += progress;

    // タイムラインデータの読み込み
    world_.timeline().add(new CameraTimeline{ &world_, "Resource/Private/Timeline/Camera/List/world1.json" });
    load_progress_ += progress;

    // アイテムデータの読み込み
    ItemDataManager::get_instance().load();
    load_progress_ += progress;

    // セーブデータの読み込み
    world_.game_save_data().load("test");
    load_progress_ += progress;
}

void GameScene::game_start() {
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);
    // シャドウマップの作成（２枚のカスケードシャドウマップ）
    static const GSuint shadow_map_size[] = { 2048, 2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // シャドウマップを適用する距離
    gsSetShadowMapDistance(60.0f);
    // カスケードシャドウのシャドウマップの分割位置を調整
    gsSetShadowMapCascadeLamda(0.7f);
    // シャドウによる光の減衰率を設定(0.0:減衰～1.0:減衰しない)
    gsSetShadowMapAttenuation(0.0f);
    // シャドウマップのポリゴンオフセットを設定する
    gsEnableShadowMapPolygonOffset();
    gsSetShadowMapPolygonOffset(2.5f, 1.0f);

    // 通常フォグの設定
    GScolor4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    const float fog_color[4]{ color.r, color.g, color.b, color.a };
    const float fog_start{ 10.0f };
    const float fog_end{ 300.0f };
    glFogi(GL_FOG_MODE, GL_LINEAR);     // 線形フォグ
    glFogfv(GL_FOG_COLOR, fog_color);   // フォグの色
    glFogf(GL_FOG_START, fog_start);    // フォグの開始位置（視点からの距離）
    glFogf(GL_FOG_END, fog_end);        // フォグの終了位置（視点からの距離）
    glEnable(GL_FOG);                   // フォグを有効にする

    // アクションスコアの初期化
    world_.action_score().init();
    // タイマーの初期化
    world_.time().init();

    // フィールドの追加
    world_.add_field(new Field{ (GSuint)OctreeID::Mesh, (GSuint)OctreeID::Collider, (GSuint)TextureID::Skybox });
    // ライトの設定
    Light* light = new Light{};
    light->position() = GSvector3{ 0.0f, 100.0f, -200.0f };
    // ライトの追加
    world_.add_light(light);

    // アタックコライダーのプールを追加
    world_.add_attack_collider_pool(new AttackColliderPool{ &world_ });

    // デフォルトカメラの追加
    world_.add_camera(new FixedCamera{ &world_, GSvector3{ 0.0f, 3.0f, -10.0f }, GSvector3{ 0.0f, 2.0f, 0.0f } });
    // タイムライン用カメラの追加
    world_.add_camera(new TimelineCamera{ &world_ });
    // エディタカメラの追加
#ifdef _DEBUG
    world_.add_camera(new EditorCamera{ &world_ });
#endif
    // プレイヤーカメラの追加
    PlayerCamera* player_camera = new PlayerCamera{ &world_ };
    world_.add_camera(player_camera);
    // プレイヤーカメラから開始
    world_.camera_transition(player_camera);

    /*
     *  TODO ローダーにする
     */

    // プレイヤーの追加
    // TODO 位置と回転を外部から取得
    world_.add_character(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f }, GSvector3{ 0.0f, 1.0f, 0.0f }, player_camera });

    // ダミーの追加
    // TODO 生成を外部から行う
    world_.add_character(new DummyEnemy{ &world_, GSvector3{ 0.0f, 0.0f, 2.0f } });

    // アイテムの生成
    // TODO 生成を外部から行う
    world_.add_actor(new ItemActor{ &world_, GSvector3{ 0.0f, 0.0f, -3.0f }, ItemData::Data{ ItemType::Weapon, 1 } });
    world_.add_actor(new ItemActor{ &world_, GSvector3{ 0.0f, 0.0f, -3.5f }, ItemData::Data{ ItemType::Weapon, 3 } });
    world_.add_actor(new ItemActor{ &world_, GSvector3{ 0.0f, 0.0f, -4.0f }, ItemData::Data{ ItemType::Weapon, 1 } });
    world_.add_actor(new ItemActor{ &world_, GSvector3{ 0.0f, 0.0f, -4.5f }, ItemData::Data{ ItemType::Weapon, 3 } });

    /*
     *  END
     */

    // シェーダーの有効化
    GameShader::get_instance().start();
    // レンダーターゲットの作成
    GamePostEffect::get_instance().start();

    // 同期
    world_.update(0.0f);
}

void GameScene::game_end() {
    // データを保存してクリア
    world_.game_save_data().save();
    world_.game_save_data().clear();
    // ワールドクリア
    world_.clear();
    // Tweenの終了
    Tween::clear();
    // 全てのエフェクトを停止する
    gsStopAllEffects();

    // シェーダーの無効化
    GameShader::get_instance().end();
    // レンダーターゲットの削除
    GamePostEffect::get_instance().end();
    // アセットの開放
    AssetsManager::get_instance().delete_asset(AssetsLoader::GAME_STAGE_ASSET_NAME);
    // メニューに戻るなら共通アセットも開放
    if (next_scene_tag_ != SceneTag::Game) AssetsManager::get_instance().delete_asset(AssetsLoader::GAME_COMMON_ASSET_NAME);

    // 次のシーンの情報を渡す
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
}
