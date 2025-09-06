#include "State/Player/PlayerSkillState.h"
#include "Actor/Player/Player.h"

PlayerSkillState::PlayerSkillState(Player& owner) :
	PlayerState(owner) {

}

void PlayerSkillState::enter() {

}

void PlayerSkillState::update(float delta_time) {
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

void PlayerSkillState::draw() const {

}

void PlayerSkillState::draw_gui() const {

}

void PlayerSkillState::exit() {

}
