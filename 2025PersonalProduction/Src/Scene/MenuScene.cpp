#include "Scene/MenuScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"

#include <gslib.h>	// tmp
#include "Assets.h"	// tmp

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

void MenuScene::load() {
    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;

    load_data();

    // 終了
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void MenuScene::start() {
	is_end_ = false;
}

void MenuScene::update(float delta_time) {
	ImGui::Begin("Scene Select Window");
	if (ImGui::Button("GameScene")) {
		is_end_ = true;
		next_scene_tag_ = SceneTag::Game;
	}
	if (ImGui::Button("TimelineEditorScene")) {
		is_end_ = true;
		next_scene_tag_ = SceneTag::TimelineEditor;
	}

	ImGui::End();
}

void MenuScene::draw() const {
	gsDrawText("menu");

	// 仮ロゴ描画
	{
		const GSrect rect{ 0.0f, 0.0f, 340.0f, 76.0f };
		const GSvector2 center{ rect.right / 2.0f, rect.bottom / 2.0f };
		Canvas::draw_texture((GSuint)TextureID::MenuLogo, GSvector2::zero(), rect, center, GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::Center);
	}
}

void MenuScene::end() {
	// Tweenの終了
	Tween::clear();

	// アセットの開放
	AssetsManager::get_instance().delete_asset("Menu");

    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;

    // 次のシーンの情報を渡す
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
}

SceneTag MenuScene::scene_tag() const {
	return SceneTag::Menu;
}

bool MenuScene::is_application_end() const {
	return false;
}

void MenuScene::reception_message(const std::string& message, std::any& param) {
	// なにも受け取らない
}

void MenuScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 1;
    const float progress = 1.0f / (float)count;

    // アセットの読み込み
    LoadAssets* asset = new LoadAssets{};
    asset->name = "Menu";
    asset->texture.push_back({ (GSuint)TextureID::MenuLogo, "Resource/Assets/Texture/menu_test.png" });
    AssetsManager::get_instance().load_asset(asset);
    load_progress_ += progress;
}
