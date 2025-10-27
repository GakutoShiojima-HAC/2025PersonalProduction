#include "State/SimpleEnemy/SimpleEnemyMoveState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"

SimpleEnemyMoveState::SimpleEnemyMoveState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyMoveState::update(float delta_time) {
    // ターゲットがいなかったらサーチに戻る
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.get_motion(SimpleEnemyStateType::Search), true);
        return;
    }

    // 判定内なら攻撃
    GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    if (to_target.magnitude() < owner_.get_info().attack_detection_radius) {
        owner_.save_prev_state();
        owner_.change_state((GSuint)SimpleEnemyStateType::Attack, owner_.get_motion(SimpleEnemyStateType::Attack), false);
        return;
    }

    GSvector3 velocity = to_target.normalized() * delta_time * owner_.get_info().move_speed * 0.1f;
    owner_.move(velocity, &to_target, 10.0f);
}
