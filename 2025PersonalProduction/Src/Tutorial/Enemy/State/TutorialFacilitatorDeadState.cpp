#include "TutorialFacilitatorDeadState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"


TutorialFacilitatorDeadState::TutorialFacilitatorDeadState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorDeadState::enter() {

}

void TutorialFacilitatorDeadState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.die();
        return;
    }
}

void TutorialFacilitatorDeadState::exit() {

}
