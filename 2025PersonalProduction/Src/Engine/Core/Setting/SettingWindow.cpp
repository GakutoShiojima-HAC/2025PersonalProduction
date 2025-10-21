#include "Engine/Core/Setting/SettingWindow.h"
#include <gslib.h>
#include "Engine/Core/Setting/Setting.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "GameConfig.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"
#include "GUI/Button/ToggleButton.h"
#include "Engine/Core/Vibration/Vibration.h"

// 歯車アイコンの回転スピード
static const float GEAR_ROTATE_SPEED{ 1.0f };

SettingWindow::SettingWindow(Setting& owner) :
    setting_{ owner } {
    // ボタンの追加
    {
        button_.add(new ToggleButton{ setting_.enable_draw_bloom(), GSvector2{ 1628.0f, 575.0f }, 38, Anchor::TopLeft, Anchor::CenterRight });
    }
    {
        button_.add(new ToggleButton{ setting_.enable_draw_ssao(), GSvector2{ 1628.0f, 661.0f }, 38, Anchor::TopLeft, Anchor::CenterRight });
    }
    {
        ToggleButton* button = new ToggleButton{ setting_.enable_vibration(), GSvector2{ 1628.0f, 779.0f }, 38, Anchor::TopLeft, Anchor::CenterRight };
        button->on_input([=] { if (setting_.is_vibration()) Vibration::get_instance().start(0.2f, 1.0f); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "戻る", GSvector2{ 1588.0f, 908.0f }, 60, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { is_end_ = true; });
        button_.add(button);
    }
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

void SettingWindow::vibration_feedback() {
    
}
