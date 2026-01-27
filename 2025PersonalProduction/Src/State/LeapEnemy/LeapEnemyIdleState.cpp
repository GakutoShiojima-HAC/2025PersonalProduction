#include "LeapEnemyIdleState.h"
#include "Actor/Enemy/LeapEnemy/LeapEnemy.h"

LeapEnemyIdleState::LeapEnemyIdleState(LeapEnemy& owner) :
    LeapEnemyState(owner) {

}

void LeapEnemyIdleState::update(float delta_time) {
    // ターゲットを見つけているか？
    const bool is_search_target = owner_.target() != nullptr;

    // 索敵範囲内に敵がいないなら待機
    if (!owner_.search_target()) {
        return;
    }

    // 始めてターゲットを見つけたら
    if (!is_search_target) {
        // 移動ステートへ
        owner_.change_state_and_motion((GSuint)LeapEnemyStateType::Move);
        return;
    }
    else {
        const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
        const float to_target_length = to_target.magnitude();

        // 近ければ移動
        if (to_target_length < owner_.my_info().attack_data.find(owner_.info().motion_attack)->second.detection_radius * 2.0f) {
            owner_.change_state_and_motion((GSuint)LeapEnemyStateType::Move);
            return;
        }
        else {
            owner_.look_target();
            owner_.change_state_and_motion((GSuint)LeapEnemyStateType::LeapStart);
            return;
        }
    }
}
