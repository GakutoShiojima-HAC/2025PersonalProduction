#include "Scene/TitleScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "GameConfig.h"
#include <gslib.h>

SceneTag NEXT_SCENE{ SceneTag::Menu };

// テキストの点滅速度
static const float TEXT_BLINK_SPEED{ 0.04f };

void TitleScene::start() {
	is_end_ = false;
    is_load_end_ = false;
    text_blink_timer_ = -1.0f;

    // タイトル用アセットの読み込み
    AssetsLoader::load_by_json("Resource/Private/Common/Assets/title.json", AssetsLoader::TITLE_ASSET_NAME);

    // 別スレッドで読み込み処理を行う
    gslib::Game::run_thread([=] {
        load_data();
    });
}

void TitleScene::update(float delta_time) {
    Tween::update(delta_time);

    // ロードが終了して何かのボタンを押したら遷移(シーン処理が少ないかつ、たまにしか来ないシーンのため毎回取得)
    if (is_load_end_ && Input::get_instance().action(InputAction::MENU_ANY)) {
        is_end_ = true;
        return;
    }

    text_blink_timer_ += delta_time * TEXT_BLINK_SPEED;
}

void TitleScene::draw() const {
    // テキストの描画
    if (is_load_end_) {
        const std::string text = Input::get_instance().is_pad() ? "PRESS  ANY  BUTTON" : "PRESS  ANY  KEY";
        const float blink_alpha = CLAMP(std::sin(text_blink_timer_), -1.0f, 0.0f) + 1.0f;
        Canvas::draw_sprite_text(
            text,
            GSvector2{ 0.0f, 300.0f },
            64,
            cFONT,
            GS_FONT_NORMAL,
            GScolor{ 1.0f, 1.0f, 1.0f, blink_alpha },
            Anchor::Center,
            Anchor::Center
        );
    }

	// 仮ロゴ描画
	{
		const GSrect rect{ 0.0f, 0.0f, 720.0f, 174.0f };
		const GSvector2 center{ rect.right / 2.0f, rect.bottom / 2.0f };
        Canvas::draw_texture((GSuint)TextureID::TitleLogo, GSvector2{ 0.0f, -100.0f }, rect, center, GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::Center);
	}
}

void TitleScene::end() {
	// Tweenの終了
	Tween::clear();

	// アセットの開放
	AssetsManager::get_instance().delete_asset(AssetsLoader::TITLE_ASSET_NAME);
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

    // 共有アセットの読み込み
    AssetsLoader::load_by_json("Resource/Private/Common/Assets/general.json", AssetsLoader::GENERAL_ASSET_NAME);

    // 読み込み終了
    is_load_end_ = true;
    text_blink_timer_ = -1.0f;
}
