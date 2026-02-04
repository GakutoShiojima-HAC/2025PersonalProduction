#include "TutorialFacilitatorMoveState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"


TutorialFacilitatorMoveState::TutorialFacilitatorMoveState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorMoveState::enter() {
    owner_.start_move();
}

void TutorialFacilitatorMoveState::update(float delta_time) {
    // ターゲットがいなかったら待機状態に戻る
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 近いか
    const bool is_in_radius = to_target_length < owner_.my_info().attack_data.find((GSuint)TutorialFacilitatorMotion::Attack)->second.detection_radius;

    if (is_in_radius) {
        owner_.look_target();
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Attack, (GSuint)TutorialFacilitatorMotion::Attack, false);
        return;
    }
    else {
        // 見失ったら待機状態に戻る
        if (to_target_length > owner_.my_info().search_length * 4.0f) {
            owner_.release_target();
            owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
            return;
        }
        // 追跡
        owner_.update_move(delta_time);
    }
}

void TutorialFacilitatorMoveState::exit() {
    owner_.move_end();
}
