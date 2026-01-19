#include "State/Viking/VikingRushState.h"
#include "Actor/Enemy/Viking/Viking.h"

VikingRushState::VikingRushState(Viking& owner) :
	VikingState(owner) {

}

void VikingRushState::enter() {
    if (owner_.start_move(owner_.target()->transform().position())) {

    }
    else {
        owner_.change_state_and_motion((GSuint)VikingStateType::Idle);
    }
    
}

void VikingRushState::update(float delta_time) {
    owner_.update_move(delta_time, 0.25f);

    if (owner_.is_move_end()) {
        owner_.change_state_and_motion((GSuint)VikingStateType::RushEnd);
        return;
    }

}

void VikingRushState::exit() {
    owner_.move_end();
}
