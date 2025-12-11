#include "TutorialFacilitatorAvoidInputState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Engine/Utils/MyMath.h"

TutorialFacilitatorAvoidInputState::TutorialFacilitatorAvoidInputState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorAvoidInputState::enter() {
    owner_.start_move();
}

void TutorialFacilitatorAvoidInputState::update(float delta_time) {
    // ターゲットがいなかったら待機状態に戻る
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
        return;
    }

    // 回避演出が起きたら次のイベントへ
    if (owner_.is_avoid_effect()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::AvoidAttackInput, (GSuint)TutorialFacilitatorMotion::Move, true);
        return;
    }

    bool is_in_radius{ false };
    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 攻撃できるかどうか
    if (owner_.is_attack_motion((GSuint)TutorialFacilitatorMotion::Attack)) {
        is_in_radius = to_target_length < owner_.my_info().attack_data.find((GSuint)TutorialFacilitatorMotion::Attack)->second.detection_radius;
    }

    // 近すぎたら見る
    if (is_in_radius) {
        // 敵の方向を向いているかどうか
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            // 判定内なら攻撃
            owner_.save_current_state();
            owner_.change_state((GSuint)TutorialFacilitatorStateType::Attack, (GSuint)TutorialFacilitatorMotion::Attack, false);
            return;
        }
        else {
            owner_.update_look_target(delta_time);
        }
    }
    else {
        // 見失ったら待機状態に戻る
        if (to_target_length > owner_.my_info().search_length * 2.5f) {
            owner_.release_target();
            owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
            return;
        }
        // 追跡
        owner_.update_move(delta_time);
    }
}

void TutorialFacilitatorAvoidInputState::exit() {
    owner_.move_end();
}
