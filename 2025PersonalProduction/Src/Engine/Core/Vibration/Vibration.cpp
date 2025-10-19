#include "Engine/Core/Vibration/Vibration.h"
#include <gslib.h>
#include "GameConfig.h"
#include "Engine/Core/Setting/Setting.h"

Vibration& Vibration::get_instance() {
    // static変数のインスタンスは１つ
    // インスタンス化も１回のみ
    static Vibration self;
    return self;
}

void Vibration::update(float delta_time) {
    if (timer_ > 0.0f) {
        timer_ -= delta_time / cFPS;
        if (timer_ <= 0.0f) end();
    }
}

void Vibration::start(float time, float strength) {
    if (!Setting::get_instance().is_vibration()) return;

    timer_ = time;
    end();
    gsXBoxPadSetVibration(0, strength, strength);
}

void Vibration::start(float time, float strength_left, float strength_right) {
    if (!Setting::get_instance().is_vibration()) return;

    timer_ = time;
    end();
    gsXBoxPadSetVibration(0, strength_left, strength_right);
}

void Vibration::end() {
    gsXBoxPadSetVibration(0, 0, 0);
}
