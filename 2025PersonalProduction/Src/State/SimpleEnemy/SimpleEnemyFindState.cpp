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

    // 探索行動の回転
    GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    GSquaternion rotation = GSquaternion::rotateTowards(
        owner_.transform().rotation(),
        GSquaternion::lookRotation(to_target),
        3.0f * delta_time
    );
    owner_.transform().rotation(rotation);

    if (MyMath::to_target_angle(owner_.collider().center, owner_.transform().forward(), owner_.target()->collider().center) <= 5.0f) {
        owner_.change_state((GSuint)SimpleEnemyStateType::Move, owner_.get_motion(SimpleEnemyStateType::Move), true);
        return;
    }

}
