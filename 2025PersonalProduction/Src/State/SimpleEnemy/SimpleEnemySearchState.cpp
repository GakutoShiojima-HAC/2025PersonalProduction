#include "State/SimpleEnemy/SimpleEnemySearchState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "GameConfig.h"
#include "Engine/Utils/MyRandom.h"

SimpleEnemySearchState::SimpleEnemySearchState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemySearchState::enter() {
    // 現在地に応じて行き先を変更(タイマー終了後判定なので逆を指定しておく)
    if ((owner_.transform().position() - owner_.origin_position()).magnitude() < 0.5f) {
        if (can_move_random()) {
            state_ = MoveState::ToRandom;
        }
    }
    else {
        if (can_move_origin()) {
            state_ = MoveState::ToOrigin;
        }
    }
    move_timer_ = get_move_time();

}

void SimpleEnemySearchState::update(float delta_time) {
    // ターゲットが見つかったら
    if (owner_.search_target()) {
        // ターゲットの方向を向く
        owner_.change_state_and_motion((GSuint)SimpleEnemyStateType::Find);
        return;
    }

    if (move_timer_ > 0.0f) {
        move_timer_ -= delta_time / cFPS;
        if (move_timer_ <= 0.0f) {
            owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.info().motion_move, true);
        }
        return;
    }

    // Random
    if (state_ == MoveState::ToRandom) {
        if (owner_.is_move_end() && can_move_origin()) {
            state_ = MoveState::ToOrigin;
            move_timer_ = get_move_time();
            owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.info().motion_idle, true);
            return;
        }
    }
    // Origin
    else {
        if (owner_.is_move_end() && can_move_random()) {
            state_ = MoveState::ToRandom;
            move_timer_ = get_move_time();
            owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.info().motion_idle, true);
            return;
        }
    }

    owner_.update_move(delta_time);    
}

bool SimpleEnemySearchState::can_move_random() {
    const GSvector3 origin = owner_.origin_position();
    const float len = owner_.my_info().search_length / 2.0f;
    const GSvector3 v = GSvector3{ len, 0.0f, len };  // TODO yは経路探索のendの高さが正しい値に修正できるまで待つ
    GSvector3 to = MyRandom::random_vec3(origin - v, origin + v);
    return owner_.start_move(to);
}

bool SimpleEnemySearchState::can_move_origin() {
    return owner_.start_move(owner_.origin_position());
}

float SimpleEnemySearchState::get_move_time() const {
    return MyRandom::random_float(3.0f, 5.0f);
}
