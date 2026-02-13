#include "MyEnemyAttackState.h"
#include "Actor/Enemy/MyEnemy.h"

MyEnemyAttackState::MyEnemyAttackState(MyEnemy& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop) :
    owner_{ owner } {
    next_state_ = next_state;
    next_motion_ = next_motion;
    motion_loop_ = motion_loop;
}

void MyEnemyAttackState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_state(next_state_, next_motion_, motion_loop_);
        return;
    }
}

void MyEnemyAttackState::exit() {
    owner_.reset_falter_count();
}

