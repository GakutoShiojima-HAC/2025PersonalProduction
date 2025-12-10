#include "AvoidEffectManager.h"
#include "Assets.h"
#include "GameConfig.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Core/World/World.h"
#include "Engine/Sound/SE.h"

// 回避演出の色
const GScolor AVOID_EFFECT_COLOR{ 0.592f, 0.627f, 1.0f, 1.0f };
// 回避演出が明ける時間
constexpr float EFFECT_RETURN_TIME{ 0.5f };

AvoidEffectManager::AvoidEffectManager(World* world) {
    world_ = world;
}

void AvoidEffectManager::update(float delta_time) {
    switch (state_) {
    case AvoidEffectManager::State::Idle: return;
    case AvoidEffectManager::State::Pause:
    {
        // 再開しない
        if (pause_timer_ < 0.0f) return;

        pause_timer_ -= delta_time / cFPS;
        // 再開
        if (pause_timer_ <= 0.0f) {
            resume();
        }
    }
        break;
    case AvoidEffectManager::State::Effect:
    {
        // 再生中
        timer_ -= delta_time / cFPS;

        // 終了
        if (timer_ <= 0.0f) {
            timer_ = EFFECT_RETURN_TIME;
            world_->set_timescale(1.0f, EFFECT_RETURN_TIME);
            Tween::color(AVOID_EFFECT_COLOR, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, EFFECT_RETURN_TIME * cFPS, [=](GScolor color) {
                world_->set_mask_color(color);
                }).name("AvoidEffectColor");
            state_ = State::Return;
        }
    }
        break;
    case AvoidEffectManager::State::Return:
    {
        // 再生中
        timer_ -= delta_time / cFPS;
        // 終了
        if (timer_ <= 0.0f) {
            world_->enable_mask_avoid_effect() = false;
            state_ = State::Idle;
        }
    }
        break;
    default: return;
    }
}

void AvoidEffectManager::clear() {
    // もしエフェクト中なら戻す
    if (is_effect()) {
        world_->set_timescale(1.0f);
        world_->set_mask_color(GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });
        world_->enable_mask_avoid_effect() = false;
    }

    state_ = State::Idle;
}

void AvoidEffectManager::start(float time, float time_scale) {
    state_ = State::Effect;
    timer_ = time;
    world_->enable_mask_avoid_effect() = true;
    world_->set_mask_color(AVOID_EFFECT_COLOR);
    if (time_scale >= 0.0f) world_->set_timescale(time_scale);
}

void AvoidEffectManager::pause(float time) {
    if (!is_effect()) return;
    pause_timer_ = time;
    prev_state_ = state_;
    state_ = State::Pause;
}

void AvoidEffectManager::resume() {
    if (!is_effect()) return;

    state_ = prev_state_;
}

bool AvoidEffectManager::is_effect() const {
    return state_ == State::Effect || state_ == State::Return || state_ == State::Pause;
}
