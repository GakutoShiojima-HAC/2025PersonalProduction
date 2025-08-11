#include "State/Player/PlayerJumpState.h"
#include "Actor/Player/Player.h"

PlayerJumpState::PlayerJumpState(Player& owner) :
	PlayerState(owner) {

}

void PlayerJumpState::enter() {

}

void PlayerJumpState::update(float delta_time) {
	owner_.update_move_air(delta_time);

	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerJumpState::draw() const {

}

void PlayerJumpState::draw_gui() const {

}

void PlayerJumpState::exit() {

}
