#include "TutorialFacilitatorAvoidAttackInputState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Engine/Utils/MyMath.h"

TutorialFacilitatorAvoidAttackInputState::TutorialFacilitatorAvoidAttackInputState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorAvoidAttackInputState::enter() {
    owner_.start_move();
}

void TutorialFacilitatorAvoidAttackInputState::update(float delta_time) {
    // É^Å[ÉQÉbÉgÇ™Ç¢Ç»Ç©Ç¡ÇΩÇÁë“ã@èÛë‘Ç…ñﬂÇÈ
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
        return;
    }

    bool is_in_radius{ false };
    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // çUåÇÇ≈Ç´ÇÈÇ©Ç«Ç§Ç©
    if (owner_.is_attack_motion((GSuint)TutorialFacilitatorMotion::Attack)) {
        is_in_radius = to_target_length < owner_.my_info().attack_data.find((GSuint)TutorialFacilitatorMotion::Attack)->second.detection_radius;
    }

    // ãﬂÇ∑Ç¨ÇΩÇÁå©ÇÈ
    if (is_in_radius) {
        // ìGÇÃï˚å¸Çå¸Ç¢ÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            // îªíËì‡Ç»ÇÁçUåÇ
            owner_.save_current_state();
            owner_.change_state((GSuint)TutorialFacilitatorStateType::Attack, (GSuint)TutorialFacilitatorMotion::Attack, false);
            return;
        }
        else {
            owner_.update_look_target(delta_time);
        }
    }
    else {
        // å©é∏Ç¡ÇΩÇÁë“ã@èÛë‘Ç…ñﬂÇÈ
        if (to_target_length > owner_.my_info().search_length * 2.5f) {
            owner_.release_target();
            owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
            return;
        }
        // í«ê’
        owner_.update_move(delta_time);
    }

}

void TutorialFacilitatorAvoidAttackInputState::exit() {
    owner_.move_end();
}
