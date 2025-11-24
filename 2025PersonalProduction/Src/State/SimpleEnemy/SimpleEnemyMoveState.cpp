#include "State/SimpleEnemy/SimpleEnemyMoveState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Utils/MyMath.h"

SimpleEnemyMoveState::SimpleEnemyMoveState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyMoveState::enter() {
    owner_.start_move();
}

void SimpleEnemyMoveState::update(float delta_time) {
    // ターゲットがいなかったらサーチに戻る
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)SimpleEnemyStateType::Search);
        return;
    }

    // 攻撃判定の中にいるかどうか
    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();
    if (to_target_length < owner_.my_info().attack_detection_radius) {
        // 敵の方向を向いているかどうか
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            // 判定内なら攻撃
            owner_.save_current_state();
            owner_.change_state_and_motion((GSuint)SimpleEnemyStateType::Attack);
            return;
        }
        else {
            owner_.update_look_target(delta_time);
        }
    }
    else {
        // 見失ったらサーチ状態に戻る
        if (to_target_length > owner_.my_info().search_length * 2.5f) {
            owner_.release_target();
            owner_.change_state_and_motion((GSuint)SimpleEnemyStateType::Search);
            return;
        }
        owner_.update_move(delta_time);
    }
}

void SimpleEnemyMoveState::exit() {
    owner_.move_end();
}
