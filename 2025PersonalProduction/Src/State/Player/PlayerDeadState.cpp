#include "State/Player/PlayerDeadState.h"
#include "Actor/Player/Player.h"

PlayerDeadState::PlayerDeadState(Player& owner) :
	PlayerState(owner) {

}

void PlayerDeadState::enter() {

}

void PlayerDeadState::update(float delta_time) {
	if (owner_.is_motion_end()) {
		owner_.die();
		return;
	}
}

void PlayerDeadState::draw() const {

}

void PlayerDeadState::draw_gui() const {

}

void PlayerDeadState::exit() {

}
