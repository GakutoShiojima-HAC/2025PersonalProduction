#include "ImpactEffectManager.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "GameConfig.h"
#include <cmath>

void ImpactEffectManager::update(float delta_time) {
    if (!is_active()) return;

    timer_ -= delta_time / cFPS;

    GamePostEffect& effect = GamePostEffect::get_instance();
    if (!is_active()) {
        effect.impact_power() = 0.0f;
        return;
    }

    //float ratio = timer_ / time_;
    //effect.impact_power() = 1.0f - std::pow(1.0f - ratio, 5.0f);
}

void ImpactEffectManager::clear() {
    timer_ = 0.0f;
}

void ImpactEffectManager::start(float time, const GSvector2& hit_screen_position) {
    time_ = time;
    timer_ = time;
    GamePostEffect& effect = GamePostEffect::get_instance();
    effect.impact_power() = 1.0f;
    effect.impact_position() = hit_screen_position;
}

bool ImpactEffectManager::is_active() const {
    return timer_ > 0.0f;
}
