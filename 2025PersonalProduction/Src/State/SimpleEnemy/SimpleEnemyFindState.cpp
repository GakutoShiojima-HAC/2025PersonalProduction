#include "State/SimpleEnemy/SimpleEnemyFindState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Utils/MyMath.h"

SimpleEnemyFindState::SimpleEnemyFindState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyFindState::update(float delta_time) {
    // ターゲットがいなかったらサーチに戻る
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.get_motion(SimpleEnemyStateType::Search), true);
        return;
    }

    owner_.update_look_target(delta_time);

    // ターゲットが見える位置に入ったら遷移
    if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
        owner_.change_state((GSuint)SimpleEnemyStateType::Move, owner_.get_motion(SimpleEnemyStateType::Move), true);
        return;
    }

}
