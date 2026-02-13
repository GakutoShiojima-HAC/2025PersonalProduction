#include "State/Viking/VikingMoveState.h"
#include "Actor/Enemy/Viking/Viking.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Utils/MyMath.h"

VikingMoveState::VikingMoveState(Viking& owner) :
	VikingState(owner) {

}

void VikingMoveState::enter() {
    owner_.start_move();
    move_timer_ = MyRandom::random_float(0.0f, 7.0f);   // あえてはみ出して特殊を引く
}

void VikingMoveState::update(float delta_time) {
    // ターゲットがいなかったらサーチに戻る
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)VikingStateType::Idle);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 一定時間の移動か索敵範囲外にいったら
    if (move_timer_ > 5.0f || to_target_length > owner_.my_info().search_length * 1.5f) {

        // 特殊攻撃
        const bool is_rush = MyRandom::random_float(0.0f, 1.0f) <= 0.5f;
        owner_.look_target();
        if (is_rush) {
            owner_.change_state_and_motion((GSuint)VikingStateType::RushCharge);
        }
        else {
            owner_.change_state_and_motion((GSuint)VikingStateType::JumpStart);
        }
        return;
    }

    // 攻撃距離に入ったら
    if (to_target_length < owner_.my_info().attack_data.find(VikingMotion::Attack)->second.detection_radius) {
        // 敵の方向を向いているかどうか
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            const bool is_attack = MyRandom::random_float(0.0f, 1.0f) <= 0.5f;
            if (is_attack) {
                owner_.change_state_and_motion((GSuint)VikingStateType::Attack);
            }
            else {
                owner_.change_state_and_motion((GSuint)VikingStateType::Block);
            }
        }
        else {
            owner_.update_look_target(delta_time);
        }
        return;
    }

    // 移動時間を計算
    move_timer_ += delta_time / cFPS;
    owner_.update_move(delta_time);
}

void VikingMoveState::exit() {
    owner_.move_end();
}
