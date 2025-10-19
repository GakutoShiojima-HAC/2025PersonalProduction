#include "Engine/Core/Camera/CameraShake.h"
#include <cmath>

void CameraShake::update(float delta_time) {
    if (is_finished_ || !enabled_) return;

    elapsed_ += delta_time / 60.0f;

    switch (type_) {
    case CameraShakeType::Shake: update_random_shake(delta_time); break;
    case CameraShakeType::ShakeV: update_shaking(delta_time); break;
    case CameraShakeType::ShakeH: update_shaking(delta_time); break;
    case CameraShakeType::SwirlClock: update_swirl(delta_time); break;
    case CameraShakeType::SwirlCounterClock: update_swirl(delta_time); break;
    case CameraShakeType::HandShake: update_hand_shake(delta_time); break;
    }

    // 揺れ時間が終了していたら揺れを終了
    if (!is_loop_ && elapsed_ >= duration_) {
        is_finished_ = true;
        strength_ = 0.0f;
        shake_offset_ = GSvector3::zero();
    }
}

void CameraShake::shake(CameraShakeType type, float duration, float strength, bool loop) {
    shake_offset_ = GSvector3::zero();	// 揺れをリセット

    // 時間が無ければ終了
    if (!loop && duration <= 0.0f) {
        is_finished_ = true;
        return;
    }

    type_ = type;	// 揺れの種類を設定
    strength_ = strength / 10.0f;
    is_loop_ = loop;
    duration_ = loop ? 1.0f : duration;	// 時間を設定(ループでは使用しない)
    elapsed_ = 0.0f;	// 経過時間をリセット
    is_finished_ = false;
}

void CameraShake::end() {
    shake_offset_ = GSvector3::zero();
    is_finished_ = true;
}

bool& CameraShake::enable() {
    return enabled_;
}

GSvector3 CameraShake::get_offset() const {
    return enabled_ ? shake_offset_ : GSvector3::zero();
}

void CameraShake::update_random_shake(float delta_time) {
    // 現在のオフセットをターゲットオフセットに向かって補間する (strength_ * 20の部分は10固定でも良いかも)
    shake_offset_.x += (swing_random_offset_.x - shake_offset_.x) * (strength_ * 20) * delta_time / 60.0f;
    shake_offset_.y += (swing_random_offset_.y - shake_offset_.y) * (strength_ * 20) * delta_time / 60.0f;

    // ターゲットオフセットに着いたら
    if (std::abs(swing_random_offset_.x - shake_offset_.x) < 0.1f && std::abs(swing_random_offset_.y - shake_offset_.y) < 0.1f) {
        float decay_factor = is_loop_ ? 1.0f : (1.0f - elapsed_ / duration_);	// 揺れの減衰率
        float strength = strength_ * decay_factor;	// 移動量

        swing_random_offset_.x = strength * ((std::rand() % 200 - 100) / 100.0f);
        swing_random_offset_.y = strength * ((std::rand() % 200 - 100) / 100.0f);   // 昔のコードを持ってきただけなので自作ランダム不使用
    }
}

void CameraShake::update_shaking(float delta_time) {
    update_swirl(delta_time);

    // 揺れの方向に応じていらない移動を打ち消す
    if (type_ == CameraShakeType::ShakeV) shake_offset_.y = 0.0f;
    else if (type_ == CameraShakeType::ShakeH) shake_offset_.x = 0.0f;
}

void CameraShake::update_swirl(float delta_time) {
    float decay_factor = is_loop_ ? 1.0f : (1.0f - elapsed_ / duration_);	// 揺れの減衰率
    float angle = elapsed_ * (type_ == CameraShakeType::SwirlClock ? 40.0f : -40.0f);	// 回転角度
    float radius = strength_ * decay_factor;	// 回転量

    shake_offset_.x = radius * std::cos(angle);
    shake_offset_.y = radius * std::sin(angle);
}

void CameraShake::update_hand_shake(float delta_time) {
    float decay_factor = is_loop_ ? 1.0f : (1.0f - elapsed_ / duration_);	// 揺れの減衰率
    float strength = strength_ * decay_factor;	// 移動量

    // 細かくランダムな揺れを生成
    shake_offset_.x = gsRandf(-0.2f, 0.2f) * strength * decay_factor;
    shake_offset_.y = gsRandf(-0.2f, 0.2f) * strength * decay_factor;
}
