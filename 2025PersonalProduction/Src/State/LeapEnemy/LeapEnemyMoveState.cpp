#include "LeapEnemyMoveState.h"
#include "Actor/Enemy/LeapEnemy/LeapEnemy.h"
#include "Engine/Utils/MyMath.h"

LeapEnemyMoveState::LeapEnemyMoveState(LeapEnemy& owner) :
    LeapEnemyState(owner) {

}

void LeapEnemyMoveState::enter() {
    owner_.start_move();
    move_timer_ = 0.0f;
}

void LeapEnemyMoveState::update(float delta_time) {
    // ターゲットがいなかったらサーチに戻る
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)LeapEnemyStateType::Idle);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 一定時間の移動か索敵範囲外にいったら
    if (move_timer_ > 5.0f || to_target_length > owner_.my_info().search_length * 1.5f) {
        owner_.look_target();
        owner_.change_state_and_motion((GSuint)LeapEnemyStateType::LeapStart);
        return;
    }

    // 攻撃距離に入ったら
    if (to_target_length < owner_.my_info().attack_data.find(owner_.info().motion_attack)->second.detection_radius) {
        // 敵の方向を向いているかどうか
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            owner_.change_state_and_motion((GSuint)LeapEnemyStateType::Attack);
            return;
        }
        else {
            owner_.update_look_target(delta_time);
        }
    }

    // 移動時間を計算
    move_timer_ += delta_time / cFPS;
    owner_.update_move(delta_time);
}

void LeapEnemyMoveState::exit() {
    owner_.move_end();
}
