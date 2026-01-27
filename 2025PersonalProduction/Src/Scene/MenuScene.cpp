#include "Scene/MenuScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "Engine/Sound/BGMManager.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

#include "State/Scene/SceneState.h"
#include "State/Scene/SceneStartState.h"
#include "State/Scene/SceneEndState.h"
#include "State/Scene/SceneOriginalState.h"
#include "State/Scene/SceneSettingState.h"
#include "State/Scene/SceneMenuState.h"

MenuScene::MenuScene() {
    add_state();
}

void MenuScene::load() {
    load_data();

    // 終了
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void MenuScene::start() {
	is_end_ = false;
    is_app_end_ = false;
    next_scene_tag_ = SceneTag::Game;

    // シェーダー上書きを開始
    GameShader::get_instance().begin();
    // レンダーターゲットの作成
    game_post_effect_.create();

    BGMManager::get_instance().play((GSuint)BGMID::Main);
    change_state((GSuint)SceneStateType::Start);
}

void MenuScene::update(float delta_time) {
    Tween::update(delta_time);
    state_.update(delta_time);

#ifdef _DEBUG
    ImGui::Begin("Scene Select Window");
    if (ImGui::Button("TimelineEditorScene")) {
        is_end_ = true;
        next_scene_tag_ = SceneTag::TimelineEditor;
    }
    ImGui::End();
#endif
}

void MenuScene::draw() const {
    game_post_effect_.begin();

    // draw 背景(octree)

    game_post_effect_.end();

    GSuint current{ 0 };    // 背景描画するならポストエフェクトを適用する ( = apply)

    game_post_effect_.begin_gui(current);

    // 背景描画
    ScreenData& data = Screen::get_instance().get_current_data();
    Canvas::draw_texture((GSuint)TextureID::MenuBackGroundTexture, GSvector2::zero(), GSrect{ 0.0f, 0.0f, (float)data.width_px, (float)data.height_px });

    state_.draw();

    game_post_effect_.end_gui();
    current = game_post_effect_.apply_dissolve(current);

    // 最終結果を描画
    game_post_effect_.draw(current);
}

void MenuScene::end() {
	// Tweenの終了
	Tween::clear();

    // シェーダー上書きを終了
    GameShader::get_instance().end();
    // レンダーターゲットの解放
    game_post_effect_.release();
	// アセットの開放
	AssetsManager::get_instance().delete_asset(AssetsLoader::MENU_ASSET_NAME);

    // 次のシーンの情報を渡す
    {
        std::any data = next_scene_tag_;
        scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
    }
    // セーブデータのフォルダ
    {
        std::string folder = "test";
        std::any data = folder;
        scene_manager_.send_message(SceneTag::Game, "LoadSaveDataName", data);
    }

    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;
}

SceneTag MenuScene::scene_tag() const {
	return SceneTag::Menu;
}

void MenuScene::reception_message(const std::string& message, std::any& param) {
	// なにも受け取らない
}

void MenuScene::add_state() {
    state_.add_state((GSuint)SceneStateType::Original, make_shared<SceneOriginalState>(*this));
    state_.add_state((GSuint)SceneStateType::Start, make_shared<SceneStartState>(*this));
    state_.add_state((GSuint)SceneStateType::End, make_shared<SceneEndState>(*this));
    state_.add_state((GSuint)SceneStateType::Setting, make_shared<SceneSettingState>(*this, SceneStateType::MenuScene));
    state_.add_state((GSuint)SceneStateType::MenuScene, make_shared<SceneMenuState>(*this));
}

void MenuScene::original_update(float delta_time) {
    // メニューシーンではオリジナルステートを使わないので遷移
    if (state_.get_current_state() == (GSuint)SceneStateType::Original) change_state((GSuint)SceneStateType::MenuScene);
}

void MenuScene::original_draw() const {
    // GUIでの固定描画があるならここへ
}

void MenuScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 1;
    const float progress = 1.0f / (float)count;


    // アセットの読み込み
    AssetsLoader::load_by_json("Resource/Private/Common/Assets/menu.json", AssetsLoader::MENU_ASSET_NAME);
    load_progress_ += progress;
}
