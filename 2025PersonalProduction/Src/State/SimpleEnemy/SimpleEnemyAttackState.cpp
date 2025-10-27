#include "State/SimpleEnemy/SimpleEnemyAttackState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"

SimpleEnemyAttackState::SimpleEnemyAttackState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyAttackState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_prev_state();
        return;
    }
}
