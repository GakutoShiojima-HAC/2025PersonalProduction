#include "Engine/Core/Setting/SettingWindow.h"
#include <gslib.h>
#include "Engine/Core/Setting/Setting.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "GameConfig.h"
#include "Assets.h"
#include "GUI/Button/TextureFunctionButton.h"

// 歯車アイコンの回転スピード
static const float GEAR_ROTATE_SPEED{ 1.0f };

SettingWindow::SettingWindow(Setting& owner) :
    setting_{ owner } {
    
}

void SettingWindow::start() {
    is_end_ = false;

    rotate_gear_ = 0.0f;
    button_.start();
}

void SettingWindow::update(float delta_time) {
    Input& input = Input::get_instance();

    // 強制終了ボタン
    if (input.action(InputAction::APP_Pause)) {
        is_end_ = true;
        return;
    }

    rotate_gear_ += delta_time  * GEAR_ROTATE_SPEED;
    button_.update(delta_time);
}

void SettingWindow::draw() const {
    Canvas::draw_texture((GSuint)TextureID::SettingWindowTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    Canvas::draw_texture((GSuint)TextureID::GearIconTexture, GSvector2{ 272.0f, 188.0f }, GSrect{ 0.0f, 0.0f, 96.0f, 96.0f },
        GSvector2{ 96.0f / 2.0f, 96.0f / 2.0f }, GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, rotate_gear_);

    Canvas::draw_texture(Input::get_instance().is_pad() ? (GSuint)TextureID::ButtonSelectGamePadTexture : (GSuint)TextureID::ButtonSelectMouseTexture,
        GSvector2{ 284.0f, 877.0f }, GSrect{ 0.0f, 0.0f, 335.0f, 64.0f });

    button_.draw();
}

bool SettingWindow::is_end() const {
    return is_end_;
}
