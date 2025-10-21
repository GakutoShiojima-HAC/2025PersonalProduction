#include "State/Scene/SceneGuideState.h"
#include <cmath>
#include "Engine/Core/Scene/StandardScene.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "Engine/Core/Input/Input.h"
#include "GUI/Button/TextFunctionButton.h"

// パッドアイコンの拡縮スピード
static const float GAMEPAD_SCALE_SPEED{ 0.05f };
// パッドアイコンの最大拡張量(もとに加算)
static const float GAMEPAD_MAX_SCALE{ 0.1f };

SceneGuideState::SceneGuideState(StandardScene& owner, SceneStateType return_scene) :
    owner_{ owner },
    return_scene_{ return_scene } {
    // ボタンの追加
    {
        TextFunctionButton* button = new TextFunctionButton{ "戻る", GSvector2{ 1588.0f, 908.0f }, 60, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.change_state((GSuint)return_scene_); });
        button_.add(button);
    }
}

void SceneGuideState::enter() {
    scale_gamepad_ = 0.0f;
    button_.start();
}

void SceneGuideState::update(float delta_time) {
    Input& input = Input::get_instance();

    // 強制終了ボタン
    if (input.action(InputAction::APP_Pause)) {
        owner_.change_state((GSuint)return_scene_);
        return;
    }

    scale_gamepad_ += delta_time * GAMEPAD_SCALE_SPEED;
    button_.update(delta_time);
}

void SceneGuideState::draw() const {
    owner_.original_draw();

    Canvas::draw_texture((GSuint)TextureID::GameGuideWindowTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    float scale = (std::sin(scale_gamepad_) + 1.0f) / 2.0f * GAMEPAD_MAX_SCALE;
    Canvas::draw_texture((GSuint)TextureID::GamePadIconTexture, GSvector2{ 272.0f, 178.0f }, GSrect{ 0.0f, 0.0f, 96.0f, 96.0f },
        GSvector2{ 96.0f / 2.0f, 96.0f / 2.0f }, GSvector2::one() + GSvector2::one() * scale);

    button_.draw();
}
