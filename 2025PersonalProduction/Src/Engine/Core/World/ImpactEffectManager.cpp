#include "ImpactEffectManager.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "GameConfig.h"

void ImpactEffectManager::update(float delta_time) {
    timer_ -= delta_time / cFPS;


    if (!is_active()) {
        GamePostEffect& effect = GamePostEffect::get_instance();
        effect.impact_power() = 0.0f;
        return;
    }
}

void ImpactEffectManager::clear() {
    timer_ = 0.0f;
}

void ImpactEffectManager::start(float time) {
    time_ = time;
    timer_ = time;
    GamePostEffect& effect = GamePostEffect::get_instance();
    effect.impact_power() = 1.0f;
}

bool ImpactEffectManager::is_active() const
{
    return timer_ > 0.0f;
}
