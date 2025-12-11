#include "TutorialFacilitatorHurtState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"

TutorialFacilitatorHurtState::TutorialFacilitatorHurtState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorHurtState::enter() {

}

void TutorialFacilitatorHurtState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_prev_state();
        return;
    }
}

void TutorialFacilitatorHurtState::exit() {

}
