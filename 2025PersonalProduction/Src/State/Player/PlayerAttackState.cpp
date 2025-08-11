#include "State/Player/PlayerAttackState.h"
#include "Actor/Player/Player.h"

PlayerAttackState::PlayerAttackState(Player& owner) :
	PlayerState(owner) {

}

void PlayerAttackState::enter() {

}

void PlayerAttackState::update(float delta_time) {
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

	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerAttackState::draw() const {

}

void PlayerAttackState::draw_gui() const {

}

void PlayerAttackState::exit() {

}
