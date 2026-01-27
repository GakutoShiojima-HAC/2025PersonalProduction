#include "ImpactEffectManager.h"
#include "GameConfig.h"

void ImpactEffectManager::update(float delta_time) {
    timer_ -= delta_time / cFPS;
}

void ImpactEffectManager::clear() {
    timer_ = 0.0f;
}

void ImpactEffectManager::start(float time) {
    timer_ = time;
}

bool ImpactEffectManager::is_active() const
{
    return timer_ > 0.0f;
}
