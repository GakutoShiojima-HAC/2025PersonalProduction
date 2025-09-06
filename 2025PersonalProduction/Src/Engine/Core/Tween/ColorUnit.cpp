#include "Engine/Core/Tween/ColorUnit.h"

ColorUnit::ColorUnit(const GScolor& from, const GScolor& to, float duration,
    std::function<void(const GScolor&)> update_func) :
    TweenUnit{ duration },
    from_{ from },
    to_{ to },
    update_func_{ update_func } { }

void ColorUnit::on_update(float progress) {
    update_func_(LERP(progress, from_, to_));
}