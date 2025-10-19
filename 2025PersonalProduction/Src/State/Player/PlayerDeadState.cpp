#include "State/Player/PlayerDeadState.h"
#include "Actor/Player/Player.h"

PlayerDeadState::PlayerDeadState(Player& owner) :
	PlayerState(owner) {

}

void PlayerDeadState::update(float delta_time) {
    // モーションが終了したら完全に死亡
	if (owner_.is_motion_end()) {
		owner_.die();
		return;
	}
}
