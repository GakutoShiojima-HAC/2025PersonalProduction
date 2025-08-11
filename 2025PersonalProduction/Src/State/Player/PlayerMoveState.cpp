#include "State/Player/PlayerMoveState.h"
#include "Actor/Player/Player.h"

PlayerMoveState::PlayerMoveState(Player& owner) :
	PlayerState(owner) {

}

void PlayerMoveState::enter() {

}

void PlayerMoveState::update(float delta_time) {
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
}

void PlayerMoveState::draw() const {

}

void PlayerMoveState::draw_gui() const {

}

void PlayerMoveState::exit() {

}
