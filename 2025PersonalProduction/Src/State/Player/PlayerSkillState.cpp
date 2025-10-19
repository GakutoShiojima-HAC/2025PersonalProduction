#include "State/Player/PlayerSkillState.h"
#include "Actor/Player/Player.h"

PlayerSkillState::PlayerSkillState(Player& owner) :
	PlayerState(owner) {

}

void PlayerSkillState::update(float delta_time) {
    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}
