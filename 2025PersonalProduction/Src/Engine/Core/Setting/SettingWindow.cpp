#include "Engine/Core/Setting/SettingWindow.h"
#include <gslib.h>
#include <cmath>
#include "Engine/Core/Setting/Setting.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "GameConfig.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"
#include "GUI/Button/ToggleButton.h"
#include "GUI/Button/TextureFunctionButton.h"
#include "Engine/Core/Vibration/Vibration.h"
#include "Engine/Sound/SE.h"

// 歯車アイコンの回転スピード
static const float GEAR_ROTATE_SPEED{ 1.0f };

SettingWindow::SettingWindow(Setting& owner) :
    setting_{ owner } {
    // ボタンの追加
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ArrowIconTexture, GSvector2{ 1461.0f, 265.0f }, GSrect{ 0.0f, 0.0f, 27.0f, 27.0f }, TextureFunctionButton::Angle::Rotate180 };
        button->on_input([=] { setting_.sens_ratio() = CLAMP(setting_.sens_ratio() - 0.1f, 0.0f, 2.0f); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ArrowIconTexture, GSvector2{ 1605.0f, 239.0f }, GSrect{ 0.0f, 0.0f, 27.0f, 27.0f } };
        button->on_input([=] { setting_.sens_ratio() = CLAMP(setting_.sens_ratio() + 0.1f, 0.0f, 2.0f); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ArrowIconTexture, GSvector2{ 1461.0f, 383.0f }, GSrect{ 0.0f, 0.0f, 27.0f, 27.0f }, TextureFunctionButton::Angle::Rotate180 };
        button->on_input([=] { gsSetVolumeBGM(CLAMP(gsGetVolumeBGM() - 0.1f, 0.0f, 1.0f)); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ArrowIconTexture, GSvector2{ 1605.0f, 355.0f }, GSrect{ 0.0f, 0.0f, 27.0f, 27.0f } };
        button->on_input([=] { gsSetVolumeBGM(CLAMP(gsGetVolumeBGM() + 0.1f, 0.0f, 1.0f)); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ArrowIconTexture, GSvector2{ 1461.0f, 469.0f }, GSrect{ 0.0f, 0.0f, 27.0f, 27.0f }, TextureFunctionButton::Angle::Rotate180 };
        button->on_input([=] { SE::set_master_volume(CLAMP(SE::get_master_volume() - 0.1f, 0.0f, 1.0f)); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ArrowIconTexture, GSvector2{ 1605.0f, 443.0f }, GSrect{ 0.0f, 0.0f, 27.0f, 27.0f } };
        button->on_input([=] { SE::set_master_volume(CLAMP(SE::get_master_volume() + 0.1f, 0.0f, 1.0f)); });
        button_.add(button);
    }
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
    const GScolor white{ 1.0f, 1.0f, 1.0f, 1.0f };

    Canvas::draw_texture((GSuint)TextureID::SettingWindowTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    Canvas::draw_texture((GSuint)TextureID::GearIconTexture, GSvector2{ 272.0f, 188.0f }, GSrect{ 0.0f, 0.0f, 96.0f, 96.0f },
        GSvector2{ 96.0f / 2.0f, 96.0f / 2.0f }, GSvector2::one(), white, rotate_gear_);

    Canvas::draw_texture(Input::get_instance().is_pad() ? (GSuint)TextureID::ButtonSelectGamePadTexture : (GSuint)TextureID::ButtonSelectMouseTexture,
        GSvector2{ 284.0f, 877.0f }, GSrect{ 0.0f, 0.0f, 335.0f, 64.0f });

    auto get_ratio_string = [&](float ratio) {
        return std::to_string(get_ratio(ratio)) + "％";
    };

    auto draw_ratio = [get_ratio_string, white](float ratio, const GSvector2& position) {
        Canvas::draw_sprite_text(
            get_ratio_string(ratio),
            position,
            38,
            cFONT,
            GS_FONT_NORMAL,
            white,
            Anchor::TopLeft,
            Anchor::Center
        );
    };
    draw_ratio(setting_.sens_ratio(), GSvector2{ 1534.0f, 252.0f });
    draw_ratio(gsGetVolumeBGM(), GSvector2{ 1534.0f, 370.0f });
    draw_ratio(SE::get_master_volume(), GSvector2{ 1534.0f, 456.0f });

    button_.draw();
    Input::get_instance().draw_cursor();
}

bool SettingWindow::is_end() const {
    return is_end_;
}

int SettingWindow::get_ratio(float value) const {
    float number = value;
    number *= std::pow(10, 2.0);
    number = std::round(number);
    number /= std::pow(10, 2.0);

    return number * 100;
}
