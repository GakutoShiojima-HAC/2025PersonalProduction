#include "State/SimpleEnemy/SimpleEnemyHurtState.h"
#include "Actor/Enemy/SimpleEnemy.h"

SimpleEnemyHurtState::SimpleEnemyHurtState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyHurtState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_prev_state();
        return;
    }
}
