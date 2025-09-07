#include "Scene/TitleScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"

#include <gslib.h>	// tmp
#include "Assets.h"	// tmp

void TitleScene::start() {
	is_end_ = false;

	// tmp
	{
		LoadAssets* asset = new LoadAssets{};
		asset->name = "Title";
		asset->texture.push_back({ (GSuint)TextureID::TitleLogo, "Resource/Assets/Texture/titlelogo_test.png" });
		AssetsManager::get_instance().load_assets(asset);
	}

	{
		LoadAssets* asset = new LoadAssets{};
		asset->name = "Menu";
		asset->texture.push_back({ (GSuint)TextureID::MenuLogo, "Resource/Assets/Texture/menu_test.png" });
		AssetsManager::get_instance().load_assets_async(asset);
	}
}

void TitleScene::update(float delta_time) {
	// change scene
	if (!AssetsManager::get_instance().is_async_load()) {
		is_end_ = true;
	}
	else {
		printf("A\n");
	}
	
	// tmp scene end
	if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;
}

void TitleScene::draw() const {
	gsDrawText("title");

	// âºÉçÉSï`âÊ
	{
		const GSrect rect{ 0.0f, 0.0f, 507.0f, 84.0f };
		const GSvector2 center{ rect.right / 2.0f, rect.bottom / 2.0f };
		Canvas::draw_texture((GSuint)TextureID::TitleLogo, GSvector2::zero(), rect, center, GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::Center);
	}
}

void TitleScene::end() {
	// TweenÇÃèIóπ
	Tween::clear();

	// tmp
	AssetsManager::get_instance().delete_assets("Title");
}

bool TitleScene::is_end() const {
	return is_end_;
}

SceneTag TitleScene::scene_tag() const {
	return SceneTag::Title;
}

SceneTag TitleScene::next_scene_tag() const {
	return SceneTag::Menu;
}

bool TitleScene::is_application_end() const {
	return false;
}

void TitleScene::reception_message(const std::string& message, void* param) {
	// Ç»Ç…Ç‡éÛÇØéÊÇÁÇ»Ç¢
}
