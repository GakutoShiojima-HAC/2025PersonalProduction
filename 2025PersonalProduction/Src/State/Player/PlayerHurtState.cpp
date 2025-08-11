#include "State/Player/PlayerHurtState.h"
#include "Actor/Player/Player.h"

PlayerHurtState::PlayerHurtState(Player& owner) :
	PlayerState(owner) {

}

void PlayerHurtState::enter() {

}

void PlayerHurtState::update(float delta_time) {
	if (owner_.is_avoid()) {
		owner_.change_state((GSuint)PlayerStateType::Avoid, owner_.get_avoid_motion(), false);
		owner_.on_avoid();
		return;
	}

	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerHurtState::draw() const {

}

void PlayerHurtState::draw_gui() const {

}

void PlayerHurtState::exit() {

}
