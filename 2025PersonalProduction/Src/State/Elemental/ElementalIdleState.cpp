#include "ElementalIdleState.h"
#include "Actor/Enemy/Elemental/Elemental.h"
#include "Engine/Utils/MyRandom.h"

ElementalIdleState::ElementalIdleState(Elemental& owner) :
    ElementalState(owner) {

}

void ElementalIdleState::update(float delta_time) {
    // ターゲットを見つけているか？
    const bool is_search_target = owner_.target() != nullptr;

    // 索敵範囲内に敵がいないなら待機
    if (!owner_.search_target()) {
        return;
    }

    // 始めてターゲットを見つけたら
    if (!is_search_target) {
        // 移動ステートへ
        owner_.change_state_and_motion((GSuint)ElementalStateType::Angry);
        return;
    }
    else {
        const float state_rate = MyRandom::random_float(0.0f, 1.0f);
        if (state_rate > 0.3f) {
            owner_.change_state_and_motion((GSuint)ElementalStateType::MoveF);
            return;
        }
        else {
            owner_.look_target();
            owner_.change_state_and_motion((GSuint)ElementalStateType::Spell);
            return;
        }
    }
}
