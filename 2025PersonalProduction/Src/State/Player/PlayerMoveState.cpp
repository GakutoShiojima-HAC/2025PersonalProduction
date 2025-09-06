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
		owner_.on_avoid();
		owner_.change_state((GSuint)PlayerStateType::Avoid);
		return;
	}

	if (owner_.is_skill()) {
		owner_.on_skill();
		owner_.change_state((GSuint)PlayerStateType::Skill, owner_.get_skill_motion(), false);
		return;
	}
	
	if (owner_.is_attack()) {
		owner_.on_attack();
		owner_.change_state((GSuint)PlayerStateType::Attack, owner_.get_attack_motion(), false);
		return;
	}

	if (owner_.is_jump()) {
		owner_.on_jump();
		owner_.change_state((GSuint)PlayerStateType::Jump, (GSuint)PlayerMotion::Jump, false);
		return;
	}

	// TODO インタラクトできるならインタラクトすること
	/*if (owner_.is_interact()) {
		owner_.change_state((GSuint)PlayerStateType::Interact, owner_.get_current_motion(), false);
		owner_.on_interact();
		return;
	}*/
}

void PlayerMoveState::draw() const {

}

void PlayerMoveState::draw_gui() const {

}

void PlayerMoveState::exit() {

}
