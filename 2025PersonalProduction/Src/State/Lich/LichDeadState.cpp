#include "State/Lich/LichDeadState.h"
#include "Actor/Enemy/Lich/Lich.h"

LichDeadState::LichDeadState(Lich& owner) :
	LichState(owner) {

}

void LichDeadState::enter() {

}

void LichDeadState::update(float delta_time) {
    if (owner_.is_motion_end()) owner_.die();
}

void LichDeadState::exit() {

}
