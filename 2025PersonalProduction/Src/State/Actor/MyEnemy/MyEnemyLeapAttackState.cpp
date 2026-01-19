#include "MyEnemyLeapAttackState.h"
#include "Actor/Enemy/MyEnemy.h"

MyEnemyLeapAttackState::MyEnemyLeapAttackState(MyEnemy& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop, float jump_height) :
    owner_{ owner } {
    next_state_ = next_state;
    next_motion_ = next_motion;
    motion_loop_ = motion_loop;
    jump_height_ = jump_height;
}

void MyEnemyLeapAttackState::enter() {
    // ターゲットチェック
    if (owner_.target() == nullptr) {
        return;
    }

    const GSvector3 from = owner_.transform().position();
    const GSvector3 to = owner_.target()->transform().position();

    const float min_height = std::max(from.y, to.y);
    const float jump_height = min_height + jump_height_;

    const const float gravity = owner_.gravity();
    const const float gravity_per_frame = (gravity > 0.0f ? gravity : 0.981f) / cFPS;

    const float rise_height = jump_height - from.y;
    // 初速
    const float vy = std::sqrt(2.0f * gravity_per_frame * rise_height);

    const float t_up = vy / gravity_per_frame;

    // 下降にかかる高さ
    const float fall_height = jump_height - to.y;
    // 下降にかかる時間
    const float t_down = std::sqrt(2.0f * fall_height / gravity_per_frame);

    // 総滞空時間
    const  float total_time = t_up + t_down;

    const GSvector3 diff = to - from;
    const float vx = diff.x / total_time;
    const float vz = diff.z / total_time;

    const GSvector3 force = GSvector3{ vx, vy, vz } * 1.0f;

    owner_.grounded() = false;
    owner_.velocity() = force;
}

void MyEnemyLeapAttackState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        next_state();
        return;
    }
}

void MyEnemyLeapAttackState::exit() {

}

void MyEnemyLeapAttackState::next_state() {
    owner_.change_state(next_state_, next_motion_, motion_loop_);
}
