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
#include "Stage/StageFile.h"

// 一時的 ローダーやマネージャーを作ったら不要
#include <gslib.h>	// シーン終了用
#include "Actor/Enemy/DummyEnemy.h"	// アクターローダーを作るべし
#include "Actor/ItemActor/ItemActor.h"  //アクターローダーを作るべし

#ifdef _DEBUG
#include "Camera/EditorCamera.h"
#endif

#include "State/Scene/SceneState.h"
#include "State/Scene/SceneOriginalState.h"
#include "State/Scene/SceneGamePauseState.h"
#include "State/Scene/SceneSettingState.h"
#include "State/Scene/SceneGuideState.h"
#include "State/Scene/SceneGameResultState.h"
#include "State/Scene/SceneGameEndState.h"

GameScene::GameScene() {
    add_state();
}

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

    next_scene_tag_ = SceneTag::Menu;

    game_start();

    change_state((GSuint)SceneStateType::Original);
}

void GameScene::update(float delta_time) {
    // タイムスケールを考慮した値渡し
    if (state_.get_current_state() == (GSuint)SceneStateType::Original) Tween::update(delta_time, delta_time * world_.timescale());
    else Tween::update(delta_time);

    state_.update(delta_time);
}

void GameScene::draw() const {
    state_.draw();
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
    if (message == "LoadSaveDataName" && param.type() == typeid(std::string)) {
        load_savedata_name_ = std::any_cast<std::string>(param);
    }
}

void GameScene::add_state() {
    state_.add_state((GSuint)SceneStateType::Original, make_shared<SceneOriginalState>(*this));
    state_.add_state((GSuint)SceneStateType::GamePause, make_shared<SceneGamePauseState>(*this));
    state_.add_state((GSuint)SceneStateType::Setting, make_shared<SceneSettingState>(*this, SceneStateType::GamePause));
    state_.add_state((GSuint)SceneStateType::Guide, make_shared<SceneGuideState>(*this, SceneStateType::GamePause));
    state_.add_state((GSuint)SceneStateType::GameResult, make_shared<SceneGameResultState>(*this, &world_));
    state_.add_state((GSuint)SceneStateType::GameEnd, make_shared<SceneGameEndState>(*this, &world_));
}

void GameScene::original_update(float delta_time) {
    // TODO 一時的なタイトルに戻る処理
    if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) {
        change_state((GSuint)SceneStateType::GameEnd);
        return;
    }

    // ポーズ
    if (input_.action(InputAction::APP_Pause)) {
        change_state((GSuint)SceneStateType::GamePause);
        return;
    }

    world_.update(delta_time);

    // TODO 一時的なゲーム終了処理
    {
        if (world_.count_actor_with_tag(ActorTag::Enemy) <= 0) {
            change_state((GSuint)SceneStateType::GameResult);
            return;
        }
        if (world_.count_actor_with_tag(ActorTag::Player) <= 0) {
            change_state((GSuint)SceneStateType::GameResult);
            return;
        }
    }
}

void GameScene::original_draw() const {
    world_.draw();
}

bool& GameScene::enable_draw_game_gui() {
    return world_.enable_draw_gui();
}

void GameScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 8;
    const float progress = 1.0f / (float)count;

    // 共通アセットの読み込み
    if (AssetsManager::get_instance().find(AssetsLoader::GAME_COMMON_ASSET_NAME));
    else  AssetsLoader::load_by_json("Resource/Private/Common/Assets/game.json", AssetsLoader::GAME_COMMON_ASSET_NAME);
    load_progress_ += progress;

    // アイテムデータの読み込み
    ItemDataManager::get_instance().load();
    load_progress_ += progress;

    // アクターデータの読み込み
    actor_generator_.load(&world_);
    load_progress_ += progress;

    // セーブデータの読み込み
    world_.game_save_data().load(load_savedata_name_);
    load_progress_ += progress;

    // ステージデータの読み込み
    StageFile sf;
    const std::string folder_path = world_.game_save_data().get().stage < 0 ? sf.get_path(-1) : sf.get_path(load_stage_index_);
    stage_data_.load(folder_path);
    load_progress_ += progress;

    // ステージ固有アセットの読み込み
    AssetsLoader::load_by_json(folder_path + "/asset.json", AssetsLoader::GAME_STAGE_ASSET_NAME);
    load_progress_ += progress;

    // ナビメッシュデータの読み込み
    world_.add_navmesh(new NavMeshSurface{ folder_path + "/navmesh.txt" });
    load_progress_ += progress;

    // タイムラインデータの読み込み
#ifdef _DEBUG
    world_.timeline().init(&world_, true);
#else
    world_.timeline().init(&world_, false);
#endif
    world_.timeline().load(folder_path + "/timeline.json");
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
    // シャドウの濃さを設定(0.0:濃い～1.0:薄い)
    gsSetShadowMapAttenuation(0.25f);
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
    world_.time().init(stage_data_.data().use_timer);

    // フィールドの追加
    world_.add_field(new Field{ (GSuint)OctreeID::Mesh, (GSuint)OctreeID::Collider, (GSuint)TextureID::Skybox });
    // ライトの追加
    world_.add_light(new Light{ GSvector3{ 55.0f, -180.0f, 0.0f } });

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

    // ダミーの追加
    // TODO 生成を外部から行う
#ifdef _DEBUG
    world_.add_character(new DummyEnemy{ &world_, GSvector3{ 0.0f, 0.0f, 2.0f } });
#endif

    // アイテムの生成
    // TODO 生成を外部から行う
    world_.add_actor(new ItemActor{ &world_, GSvector3{ 0.0f, 0.0f, 3.0f }, ItemData::Data{ ItemType::Weapon, 1 } });

    // アクターの生成
    actor_generator_.generate(stage_data_.data().folder + "/generate.json");

    /*
     *  END
     */

#ifndef _DEBUG
    // エディタでの動的生成のためにデバッグ中はデータを残しておく
    actor_generator_.clear();
#endif

    // シェーダーの有効化
    GameShader::get_instance().start();
    // レンダーターゲットの作成
    GamePostEffect::get_instance().start();

    // GUIの描画を有効化
    world_.enable_draw_gui() = true;

    // タイムスケールを初期化
    world_.timescale() = 1.0f;

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

#ifdef _DEBUG
    // シーン中データを残していた分ここで破棄
    actor_generator_.clear();
#endif

    // ロビーにリセット
    load_stage_index_ = 0;  

    // 次のシーンの情報を渡す
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
}
