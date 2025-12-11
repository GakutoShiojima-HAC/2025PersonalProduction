#include "TutorialFacilitatorAttackState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"

TutorialFacilitatorAttackState::TutorialFacilitatorAttackState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorAttackState::enter() {

}

void TutorialFacilitatorAttackState::update(float delta_time) {

    // ÉÇÅ[ÉVÉáÉìèIóπÇµÇΩÇÁå≥Ç…ñﬂÇÈ
    if (owner_.is_motion_end()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::AvoidAttackInput, TutorialFacilitatorMotion::Move, true);
        return;
    }
}

void TutorialFacilitatorAttackState::exit() {

}
