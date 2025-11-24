#include "State/Lich/LichSpellState.h"
#include "Actor/Enemy/Lich/Lich.h"

LichSpellState::LichSpellState(Lich& owner) :
	LichState(owner) {

}

void LichSpellState::enter() {

}

void LichSpellState::update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_state_and_motion((GSuint)LichStateType::Idle);
        return;
    }
}

void LichSpellState::exit() {

}
