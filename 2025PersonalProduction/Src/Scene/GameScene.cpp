#include "Scene/GameScene.h"
#include <GSeffect.h>
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"
#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"

#include <gslib.h>	// tmp
#include "Camera/FixedCamera.h"	// tmp
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"	// tmp
#include "Camera/TimelineCamera.h"	// tmp
#include "Camera/PlayerCamera.h"	// tmp
#include "Actor/Player/Player.h"	// tmp
#include "Camera/EditorCamera.h"	// tmp
#include "Actor/Enemy/DummyEnemy.h"	// tmp
#include "Assets.h"	// tmp

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

    // TODO Game or Menu
    next_scene_tag_ = SceneTag::Menu;

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

	world_.add_field(new Field{ (GSuint)OctreeID::Mesh, (GSuint)OctreeID::Collider, (GSuint)TextureID::Skybox });
	Light* light = new Light{};
	light->position() = GSvector3{ 0.0f, 100.0f, -200.0f };
	world_.add_light(light);
	world_.add_attack_collider_pool(new AttackColliderPool{ &world_ });

	// tmp
	world_.add_camera(new FixedCamera{ &world_, GSvector3{ 0.0f, 3.0f, -10.0f }, GSvector3{ 0.0f, 2.0f, 0.0f } });
	world_.add_camera(new TimelineCamera{ &world_ });
	world_.add_camera(new EditorCamera{ &world_ });

	// tmp
	PlayerCamera* player_camera = new PlayerCamera{ &world_ };
	world_.add_camera(player_camera);
	world_.add_character(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f }, GSvector3{ 0.0f, 1.0f, 0.0f }, player_camera });
	world_.camera_transition(player_camera);

	// tmp
	world_.add_character(new DummyEnemy{ &world_, GSvector3{ 0.0f, 0.0f, 2.0f } });

    // シェーダーの有効化
    GameShader::get_instance().start();
    // レンダーターゲットの作成
    GamePostEffect::get_instance().start();

	// 同期
	world_.update(0.0f);
}

void GameScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;

	world_.update(delta_time);

	// TODO tmp scene end func
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
	AssetsManager::get_instance().delete_asset("Game");

    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;

    // 次のシーンの情報を渡す
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
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
    const int count = 6;
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
}
