#include "Scene/MenuScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"

#include <gslib.h>	// tmp
#include "Assets.h"	// tmp

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

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

	// âºÉçÉSï`âÊ
	{
		const GSrect rect{ 0.0f, 0.0f, 340.0f, 76.0f };
		const GSvector2 center{ rect.right / 2.0f, rect.bottom / 2.0f };
		Canvas::draw_texture((GSuint)TextureID::MenuLogo, GSvector2::zero(), rect, center, GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::Center);
	}
}

void MenuScene::end() {
	// TweenÇÃèIóπ
	Tween::clear();

	// tmp
	AssetsManager::get_instance().delete_assets("Menu");
}

bool MenuScene::is_end() const {
	return is_end_;
}

SceneTag MenuScene::scene_tag() const {
	return SceneTag::Menu;
}

SceneTag MenuScene::next_scene_tag() const {
	return next_scene_tag_;
}

bool MenuScene::is_application_end() const {
	return false;
}

void MenuScene::reception_message(const std::string& message, void* param) {
	// Ç»Ç…Ç‡éÛÇØéÊÇÁÇ»Ç¢
}
