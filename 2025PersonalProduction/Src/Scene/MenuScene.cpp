#include "Scene/MenuScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
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

    // TODO load function
    // maybe async other function

    // 終了
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void MenuScene::start() {
	is_end_ = false;

	if (AssetsManager::get_instance().find("Menu") == nullptr) {
		LoadAssets* asset = new LoadAssets{};
		asset->name = "Menu";
		asset->texture.push_back({ (GSuint)TextureID::MenuLogo, "Resource/Assets/Texture/menu_test.png" });
		AssetsManager::get_instance().load_assets(asset);
	}
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

	// tmp
	AssetsManager::get_instance().delete_assets("Menu");

    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;
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
