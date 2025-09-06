#include "State/Player/PlayerAttackState.h"
#include "Actor/Player/Player.h"

PlayerAttackState::PlayerAttackState(Player& owner) :
	PlayerState(owner) {

}

void PlayerAttackState::enter() {
	next_attack_input_ = false;

	enter_next_attack_timer_ = owner_.get_enter_next_attack_animation_time();
}

void PlayerAttackState::update(float delta_time) {
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

	enter_next_attack_timer_ -= delta_time;

	if (owner_.is_attack()) {
		next_attack_input_ = true;
	}

	if (next_attack_input_ && enter_next_attack_timer_ <= 0.0f) {
		next_attack_input_ = false;
		owner_.attack_count() += 1;
		enter_next_attack_timer_ = owner_.get_enter_next_attack_animation_time();
		owner_.change_state((GSuint)PlayerStateType::Attack, owner_.get_attack_motion(), false);
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
	owner_.attack_count() = 0;
	enter_next_attack_timer_ = 0.0f;
}
