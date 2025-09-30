#include "Scene/TitleScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include <gslib.h>

#include "Assets.h"	// tmp

SceneTag NEXT_SCENE{ SceneTag::Menu };

void TitleScene::start() {
	is_end_ = false;

    // タイトル用アセットの読み込み
    LoadAssets* asset = new LoadAssets{};
    asset->name = "Title";
    asset->texture.push_back({ (GSuint)TextureID::TitleLogo, "Resource/Assets/Texture/titlelogo_test.png" });
    AssetsManager::get_instance().load_asset(asset);

    // 別スレッドで読み込み処理を行う
    gslib::Game::run_thread([=] {
        load_data();
    });
}

void TitleScene::update(float delta_time) {

}

void TitleScene::draw() const {
	gsDrawText("title");

	// 仮ロゴ描画
	{
		const GSrect rect{ 0.0f, 0.0f, 507.0f, 84.0f };
		const GSvector2 center{ rect.right / 2.0f, rect.bottom / 2.0f };
		Canvas::draw_texture((GSuint)TextureID::TitleLogo, GSvector2::zero(), rect, center, GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::Center);
	}
}

void TitleScene::end() {
	// Tweenの終了
	Tween::clear();

	// アセットの開放
	AssetsManager::get_instance().delete_asset("Title");
}

bool TitleScene::is_end() const {
	return is_end_;
}

SceneTag TitleScene::scene_tag() const {
	return SceneTag::Title;
}

SceneTag TitleScene::next_scene_tag() const {
    return NEXT_SCENE;
}

bool TitleScene::is_application_end() const {
	return false;
}

void TitleScene::reception_message(const std::string& message, std::any& param) {
	// なにも受け取らない
}

bool TitleScene::is_load_end() const {
    return true;
}

float TitleScene::load_progress() const {
    return 1.0f;
}

void TitleScene::load_data() {
    // ゲームに必要なシェーダーの読み込み
    GameShader::get_instance().load();
    GamePostEffect::get_instance().load();

    // 次のシーンのアセットを読み込み
    scene_manager_.load_scene(NEXT_SCENE);

    // 終了
    is_end_ = true;
}
