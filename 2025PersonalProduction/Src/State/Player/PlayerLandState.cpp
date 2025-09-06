#include "State/Player/PlayerLandState.h"
#include "Actor/Player/Player.h"

PlayerLandState::PlayerLandState(Player& owner) :
	PlayerState(owner) {

}

void PlayerLandState::enter() {

}

void PlayerLandState::update(float delta_time) {
	if (owner_.is_avoid()) {
		owner_.on_avoid();
		owner_.change_state((GSuint)PlayerStateType::Avoid);
		return;
	}

	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerLandState::draw() const {

}

void PlayerLandState::draw_gui() const {

}

void PlayerLandState::exit() {

}
