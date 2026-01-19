#include "State/Viking/VikingIdleState.h"
#include "Actor/Enemy/Viking/Viking.h"
#include "Engine/Utils/MyRandom.h"

VikingIdleState::VikingIdleState(Viking& owner) :
	VikingState(owner) {

}

void VikingIdleState::enter() {

}

void VikingIdleState::update(float delta_time) {
    // ターゲットを見つけているか？
    const bool is_search_target = owner_.target() != nullptr;

    // 索敵範囲内に敵がいないなら待機
    if (!owner_.search_target()) {
        return;
    }

    // 始めてターゲットを見つけたら
    if (!is_search_target) {
        // 移動ステートへ
        owner_.change_state_and_motion((GSuint)VikingStateType::Angry);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 近ければ移動
    if (to_target_length < owner_.my_info().attack_data.find(VikingMotion::Attack)->second.detection_radius * 2.0f) {
        owner_.change_state_and_motion((GSuint)VikingStateType::Move);
    }
    else {
        // 遠ければ分岐
        const float state_rate = MyRandom::random_float(0.0f, 1.0f);
        if (state_rate < 0.3f) {
            owner_.change_state_and_motion((GSuint)VikingStateType::Move);
        }
        else if (state_rate < 0.6f) {
            owner_.look_target();
            owner_.change_state_and_motion((GSuint)VikingStateType::JumpStart);
        }
        else {
            owner_.look_target();
            owner_.change_state_and_motion((GSuint)VikingStateType::RushCharge);
        }
    }
}
    
void VikingIdleState::exit() {

}
