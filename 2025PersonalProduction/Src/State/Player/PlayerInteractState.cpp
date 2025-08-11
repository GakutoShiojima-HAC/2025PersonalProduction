#include "State/Player/PlayerInteractState.h"
#include "Actor/Player/Player.h"

PlayerInteractState::PlayerInteractState(Player& owner) :
	PlayerState(owner) {

}

void PlayerInteractState::enter() {

}

void PlayerInteractState::update(float delta_time) {
	owner_.update_lockon_camera();
	owner_.update_move(delta_time);

	if (owner_.is_avoid()) {
		owner_.change_state((GSuint)PlayerStateType::Avoid, owner_.get_avoid_motion(), false);
		owner_.on_avoid();
		return;
	}

	if (owner_.is_skill()) {
		owner_.change_state((GSuint)PlayerStateType::Skill, owner_.get_skill_motion(), false);
		owner_.on_skill();
		return;
	}

	if (owner_.is_attack()) {
		owner_.change_state((GSuint)PlayerStateType::Attack, owner_.get_attack_motion(), false);
		owner_.on_attack();
		return;
	}

	if (owner_.is_jump()) {
		owner_.change_state((GSuint)PlayerStateType::Jump, (GSuint)PlayerMotion::Jump, false);
		owner_.on_jump();
		return;
	}

	if (owner_.is_interact()) {
		owner_.change_state((GSuint)PlayerStateType::Interact, (GSuint)PlayerMotion::Interact, false);
		owner_.on_interact();
		return;
	}

	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerInteractState::draw() const {

}

void PlayerInteractState::draw_gui() const {

}

void PlayerInteractState::exit() {

}
