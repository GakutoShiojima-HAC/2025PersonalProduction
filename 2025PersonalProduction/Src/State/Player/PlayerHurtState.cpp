#include "State/Player/PlayerHurtState.h"
#include "Actor/Player/Player.h"

PlayerHurtState::PlayerHurtState(Player& owner) :
	PlayerState(owner) {

}

void PlayerHurtState::enter() {

}

void PlayerHurtState::update(float delta_time) {
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

void PlayerHurtState::draw() const {

}

void PlayerHurtState::draw_gui() const {

}

void PlayerHurtState::exit() {

}
