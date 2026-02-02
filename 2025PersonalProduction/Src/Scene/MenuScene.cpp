#include "Scene/MenuScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "Engine/Sound/BGMManager.h"
#include "Engine/Utils/MyTime.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

#include "State/Scene/SceneState.h"
#include "State/Scene/SceneStartState.h"
#include "State/Scene/SceneEndState.h"
#include "State/Scene/SceneOriginalState.h"
#include "State/Scene/SceneSettingState.h"
#include "State/Scene/SceneMenuState.h"
#include "State/Scene/SceneSelectSaveDataState.h"

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
        // 条件
        const bool is_new = select_save_data_index_ < 0;
        const bool is_empty = save_data_info_.empty() || select_save_data_index_>= save_data_info_.size();

        const std::string time = MyLib::get_time_info().time_info_to_filename_string();

        const std::string folder = is_new ? time : is_empty ? time : save_data_info_[select_save_data_index_].name;
        std::any data = folder;
        scene_manager_.send_message(SceneTag::Game, "LoadSaveDataName", data);
    }    

    // セーブデータの情報を破棄
    save_data_info_.clear();
    select_save_data_index_ = -1;

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
    state_.add_state((GSuint)SceneStateType::MenuSaveDataSelect, make_shared<SceneSelectSaveDataState>(*this));
}

void MenuScene::original_update(float delta_time) {
    // メニューシーンではオリジナルステートを使わないので遷移
    if (state_.get_current_state() == (GSuint)SceneStateType::Original) change_state((GSuint)SceneStateType::MenuScene);
}

void MenuScene::original_draw() const {
    // GUIでの固定描画があるならここへ
}

const std::vector<MenuSceneSaveDataInfo>& MenuScene::get_save_data() const {
    return save_data_info_;
}

int& MenuScene::select_save_data_index() {
    return select_save_data_index_;
}

void MenuScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 2;
    const float progress = 1.0f / (float)count;


    // アセットの読み込み
    AssetsLoader::load_by_json("Resource/Private/Common/Assets/menu.json", AssetsLoader::MENU_ASSET_NAME);
    load_progress_ += progress;

    // セーブデータの情報の読み込み
    load_save_data_info();
    load_progress_ += progress;
}

void MenuScene::load_save_data_info() {
    save_data_info_.clear();
    select_save_data_index_ = -1;

    std::vector<std::string> paths;
    for (const auto& entry : fs::recursive_directory_iterator("SaveData/")) {
        const fs::path& p = entry.path();
        if (fs::is_regular_file(p)) {
            const std::string file_ext = entry.path().extension().string();
            if (file_ext == ".json") paths.push_back(entry.path().string());
        }
    }
    if (paths.empty()) return;

    for (const auto& path : paths) {
        json j;
        if (!MyJson::is_json(path, j)) continue;

        MenuSceneSaveDataInfo info;

        fs::path p(path);
        info.name = p.stem().string();
        info.time = MyJson::get_string(j, "Time", "XXXX/XX/XX/ XX:XX:XX");
        info.stage = MyJson::get_int(j, "Stage", -1);
        info.level = MyJson::get_int(j, "PlayerLevel", 1);

        save_data_info_.push_back(info);
    }
}
