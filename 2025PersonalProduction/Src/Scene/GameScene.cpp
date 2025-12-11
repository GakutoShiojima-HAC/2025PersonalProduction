#include "Scene/GameScene.h"
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
#include "Stage/StageFile.h"
#include "Engine/Sound/BGMManager.h"

#include "Actor/CinemaActor/CinemaActor.h"
#include "Actor/CinemaActor/CinemaBehavior/NormalEnemyCounter.h"

// 一時的 ローダーやマネージャーを作ったら不要
#include "Actor/Enemy/DummyEnemy.h"	// アクターローダーを作るべし

#ifdef _DEBUG
#include "Camera/EditorCamera.h"
#endif

#include "State/Scene/SceneState.h"
#include "State/Scene/SceneStartState.h"
#include "State/Scene/SceneEndState.h"
#include "State/Scene/SceneOriginalState.h"
#include "State/Scene/SceneGamePauseState.h"
#include "State/Scene/SceneSettingState.h"
#include "State/Scene/SceneGuideState.h"
#include "State/Scene/SceneGameResultState.h"

GameScene::GameScene() {
    add_state();
}

void GameScene::load() {
    load_data();

    // 終了
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void GameScene::start() {
	is_end_ = false;

    next_scene_tag_ = SceneTag::Menu;

    game_start();

    change_state((GSuint)SceneStateType::Start);
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
    // 他シーンからゲームシーンに遷移するとき、読み込むセーブデータを設定する
    if (message == "LoadSaveDataName" && param.type() == typeid(std::string)) {
        load_savedata_name_ = std::any_cast<std::string>(param);
        // 最初はロビー
        load_stage_index_ = 0;
    }
    // ロビーからステージへ移動する
    if (message == "RequestTereport" && param.type() == typeid(int)) {
        // 移動先
        load_stage_index_ = std::any_cast<int>(param);
        // 遷移条件
        if (state_.is_current_state((GSuint)SceneStateType::Original)) {
            set_next_scene(SceneTag::Game);
            change_state((GSuint)SceneStateType::End);
        }
    }
}

void GameScene::add_state() {
    state_.add_state((GSuint)SceneStateType::Start, make_shared<SceneStartState>(*this));
    state_.add_state((GSuint)SceneStateType::End, make_shared<SceneEndState>(*this));
    state_.add_state((GSuint)SceneStateType::Original, make_shared<SceneOriginalState>(*this));
    state_.add_state((GSuint)SceneStateType::GamePause, make_shared<SceneGamePauseState>(*this));
    state_.add_state((GSuint)SceneStateType::Setting, make_shared<SceneSettingState>(*this, SceneStateType::GamePause));
    state_.add_state((GSuint)SceneStateType::Guide, make_shared<SceneGuideState>(*this, SceneStateType::GamePause));
    state_.add_state((GSuint)SceneStateType::GameResult, make_shared<SceneGameResultState>(*this, &world_));
}

void GameScene::original_update(float delta_time) {
    world_.update(delta_time);

    if (state_.get_current_state() != (GSuint)SceneStateType::Original) return;

    // ポーズ
    if (input_.action(InputAction::APP_Pause)) {
        change_state((GSuint)SceneStateType::GamePause);
        return;
    }

    // ゲーム終了処理
    if (stage_data_.data().use_result) {
        if (world_.count_actor_with_tag(ActorTag::Enemy) <= 0) {
            change_state((GSuint)SceneStateType::GameResult);
            return;
        }
        if (world_.count_actor_with_tag(ActorTag::Player) <= 0) {
            change_state((GSuint)SceneStateType::GameResult);
            return;
        }
    }
    else {
        // リザルトがない(敵が出現しない)ステージで万が一死んでしまったら自動復活
        if (world_.count_actor_with_tag(ActorTag::Player) <= 0) {
            // 一応設定
            world_.player_respawner().add_point(GSvector3::zero(), GSvector3::zero());
            respawn_player();
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

void GameScene::respawn_player() {
    PlayerRespawner& respawner = world_.player_respawner();
    actor_generator_.generate("Player", respawner.respawn_position(), respawner.respawn_rotate());
    respawner.respawn_countup();    // カウントアップ

    // 入力可能状態を初期化
    input_.reset_disable_action();
}

void GameScene::set_next_stage(int id) {
    load_stage_index_ = id;
}

int GameScene::get_current_load_stage() const {
    return load_stage_index_;
}

void GameScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 12;
    const float progress = 1.0f / (float)count;

    // 進捗率更新
    auto next = [&]() {
        load_progress_ += progress;
    };

    // 共通アセットの読み込み
    if (AssetsManager::get_instance().find(AssetsLoader::GAME_COMMON_ASSET_NAME));
    else  AssetsLoader::load_by_json("Resource/Private/Common/Assets/game.json", AssetsLoader::GAME_COMMON_ASSET_NAME);
                                                                                                                                next();

    // アイテムデータの読み込み
    ItemDataManager::get_instance().load();                                                                                     next();

    // アクターデータの読み込み
    actor_generator_.load(&world_);                                                                                             next();

    // セーブデータの読み込み
    world_.game_save_data().load(load_savedata_name_);                                                                          next();

    // ステージデータのフォルダを取得
    StageFile sf;                                                                                                               next();

    // ステージデータの読み込み(チュートリアル未クリアの場合はチュートリアルを優先する)
    load_stage_index_ = world_.game_save_data().get().stage < 0 ? -1 : load_stage_index_;
    const std::string folder_path = sf.get_path(load_stage_index_);
    const StageLoadConfigData data = stage_data_.load(folder_path);                                                             next();

    // ステージ固有アセットの読み込み
    AssetsLoader::load_by_json(folder_path + "/asset.json", AssetsLoader::GAME_STAGE_ASSET_NAME);                               next();

    // ライト関係の読み込み
    gsLoadLightmap(0, data.lightmap.c_str());                                                                                   next();
    gsLoadReflectionProbe(0, data.refprobe.c_str());                                                                            next();

    // ナビメッシュデータの読み込み
    world_.add_navmesh(new NavMeshSurface{ data.navmesh });                                                                     next();

    // タイムラインデータの読み込み
#ifdef _DEBUG
    world_.timeline().init(&world_, true);  // エディタ有効
#else
    world_.timeline().init(&world_, false); // エディタ無効
#endif
    world_.timeline().load(folder_path + "/timeline.json");                                                                     next();

    // 静的エフェクトデータの読み込み
    world_.load_static_effect(folder_path + "/static_effect.json");                                                             next();
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
    gsSetShadowMapAttenuation(0.125f);
    // シャドウマップのポリゴンオフセットを設定する
    gsEnableShadowMapPolygonOffset();
    gsSetShadowMapPolygonOffset(2.5f, 1.0f);

    /*
    // 通常フォグの設定
    GScolor4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    const float fog_color[4]{ color.r, color.g, color.b, color.a };
    const float fog_start{ 50.0f };
    const float fog_end{ 300.0f };
    glFogi(GL_FOG_MODE, GL_LINEAR);     // 線形フォグ
    glFogfv(GL_FOG_COLOR, fog_color);   // フォグの色
    glFogf(GL_FOG_START, fog_start);    // フォグの開始位置（視点からの距離）
    glFogf(GL_FOG_END, fog_end);        // フォグの終了位置（視点からの距離）
    glEnable(GL_FOG);                   // フォグを有効にする
    */

    // ステージデータの取得
    const StageConfigData& config = stage_data_.data();

    // アクションスコアの初期化
    world_.action_score().init();
    // タイマーの初期化
    world_.time().init(config.use_timer);

    // フィールドの追加
    world_.add_field(new Field{ (GSuint)OctreeID::Mesh, (GSuint)OctreeID::Collider, (GSuint)TextureID::Skybox });
    // ライトの追加
    world_.add_light(new Light{ config.light_angle, GScolor{ 0.05f, 0.05f, 0.05f, 1.0f } });

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

    /*
     *  TODO ローダーにする
     */

    // ダミーの追加
    // TODO 生成を外部から行う
#ifdef _DEBUG
    //world_.add_character(new DummyEnemy{ &world_, GSvector3{ 0.0f, 0.0f, 2.0f } });
#endif

    /*
     *  END
     */

    // アクターの生成
    actor_generator_.generate(config.folder + "/generate.json");

    // 通常エネミーカウンターの生成
    if (config.use_normal_enemy_counter) {
        CinemaActor* cinema_actor = new CinemaActor{ &world_, "BreakBarrier" , true };
        cinema_actor->set_behavior(new NormalEnemyCounter{ &world_, actor_generator_.count_generate_enemy() });
        world_.add_actor(cinema_actor);
    }

#ifndef _DEBUG
    // エディタでの動的生成のためにデバッグ中はデータを残しておく
    actor_generator_.clear_no_respawn();
#endif

    // 静的エフェクトの生成
    world_.generate_static_effect();

    // シェーダー上書きを開始
    GameShader::get_instance().begin();
    // レンダーターゲットの作成
    GamePostEffect::get_instance().create();

    // GUIの描画を有効化
    world_.enable_draw_gui() = true;
    world_.enable_draw_game_info_gui() = true;
    // タイムスケールを初期化
    world_.timescale() = 1.0f;
    // 同期
    world_.update(0.0f);

    BGMManager::get_instance().play((GSuint)BGMID::Main);
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

    // 入力可能状態を初期化
    input_.reset_disable_action();

    // シェーダー上書きを終了
    GameShader::get_instance().end();
    // レンダーターゲットの解放
    GamePostEffect::get_instance().release();
    // アセットの開放
    AssetsManager::get_instance().delete_asset(AssetsLoader::GAME_STAGE_ASSET_NAME);
    // メニューに戻るなら共通アセットも開放
    if (next_scene_tag_ != SceneTag::Game) AssetsManager::get_instance().delete_asset(AssetsLoader::GAME_COMMON_ASSET_NAME);

    // アクター生成用データを完全に破棄
    actor_generator_.clear();

    // 次のシーンの情報を渡す
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);

    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;
}
