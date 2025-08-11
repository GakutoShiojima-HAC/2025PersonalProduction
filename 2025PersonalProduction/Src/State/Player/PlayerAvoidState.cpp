#include "State/Player/PlayerAvoidState.h"
#include "Actor/Player/Player.h"

PlayerAvoidState::PlayerAvoidState(Player& owner) :
	PlayerState(owner) {

}

void PlayerAvoidState::enter() {

}

void PlayerAvoidState::update(float delta_time) {
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerAvoidState::draw() const {

}

void PlayerAvoidState::draw_gui() const {

}

void PlayerAvoidState::exit() {

}
