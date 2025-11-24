#include "State/Lich/LichHurtState.h"
#include "Actor/Enemy/Lich/Lich.h"

LichHurtState::LichHurtState(Lich& owner) :
	LichState(owner) {

}

void LichHurtState::enter() {

}

void LichHurtState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_state_and_motion((GSuint)LichStateType::Idle);
        return;
    }
}

void LichHurtState::exit() {

}
