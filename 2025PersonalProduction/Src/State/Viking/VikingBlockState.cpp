#include "State/Viking/VikingBlockState.h"
#include "Actor/Enemy/Viking/Viking.h"

VikingBlockState::VikingBlockState(Viking& owner) :
	VikingState(owner) {

}

void VikingBlockState::enter() {

}

void VikingBlockState::update(float delta_time) {
    owner_.update_look_target(delta_time);


    if (owner_.state_timer() > 1.0f * cFPS) {
        const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
        const float to_target_length = to_target.magnitude();
        if (to_target_length < owner_.my_info().attack_data.find(VikingMotion::Attack)->second.detection_radius) {
            owner_.change_state_and_motion((GSuint)VikingStateType::Attack);
        }
        else {
            owner_.change_state_and_motion((GSuint)VikingStateType::Move);
        }

        return;
    }
}

void VikingBlockState::exit() {

}
