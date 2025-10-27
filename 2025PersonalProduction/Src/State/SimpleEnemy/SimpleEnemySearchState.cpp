#include "State/SimpleEnemy/SimpleEnemySearchState.h"
#include "Actor/Enemy/SimpleEnemy.h"
#include "GameConfig.h"

SimpleEnemySearchState::SimpleEnemySearchState(SimpleEnemy& owner) : SimpleEnemyState(owner) {
    // モーションを取得
    to_find_state_motion_ = owner_.get_motion(SimpleEnemyStateType::Find, &to_find_state_loop_);

    move_timer_ = 0.0f;
}

void SimpleEnemySearchState::update(float delta_time) {
    // ターゲットが見つかったら
    if (owner_.search_target()) {
        // ターゲットの方向を向く
        owner_.change_state((GSuint)SimpleEnemyStateType::Find, to_find_state_motion_, to_find_state_loop_);
        return;
    }

    // 見つけていない間は探索行動を行う
    move_timer_ += delta_time / cFPS;
    if (move_timer_ > 5.0f) {
        // 右を向く
        rotate_foward_ = owner_.transform().right();
        move_timer_ = 0.0f;
    }

    // 探索行動の回転
    GSquaternion rotation = GSquaternion::rotateTowards(
        owner_.transform().rotation(),
        GSquaternion::lookRotation(rotate_foward_),
        3.0f
    );
    owner_.transform().rotation(rotation);
}
