#include "MyEnemyRetreatState.h"
#include "Actor/Enemy/MyEnemy.h"

MyEnemyRetreatState::MyEnemyRetreatState(MyEnemy& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop, float retreat_distance) :
    owner_{ owner } {
    next_state_ = next_state;
    next_motion_ = next_motion;
    motion_loop_ = motion_loop;
    retreat_distance_ = retreat_distance;
}

void MyEnemyRetreatState::enter() {
    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 範囲内にいるなら後退りする
    if (to_target_length < retreat_distance_) {
        const GSvector3 to = owner_.transform().position() - owner_.transform().forward() * 2.0f;
        if (owner_.start_move(to)) {

        }
        else {
            owner_.move_end();
            next_state();
        }
    }
    else {
        next_state();
    }
}

void MyEnemyRetreatState::update(float delta_time) {
    owner_.update_move(delta_time);
    owner_.look_target();   // 後退りなのでターゲットを見る

    if (owner_.is_move_end()) {
        next_state();
        return;
    }
}

void MyEnemyRetreatState::exit() {
    owner_.move_end();
}

void MyEnemyRetreatState::next_state() {
    owner_.change_state(next_state_, next_motion_, motion_loop_);
}
