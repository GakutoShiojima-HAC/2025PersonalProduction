#include "State/SimpleEnemy/SimpleEnemyDeadState.h"
#include "Actor/Enemy/SimpleEnemy.h"

SimpleEnemyDeadState::SimpleEnemyDeadState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyDeadState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.die();
        return;
    }
}
