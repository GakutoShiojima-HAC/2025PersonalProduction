#include "State/Player/PlayerFallState.h"
#include "Actor/Player/Player.h"

PlayerFallState::PlayerFallState(Player& owner) :
	PlayerState(owner) {

}

void PlayerFallState::update(float delta_time) {
    // 空中移動の更新
	owner_.update_move_air(delta_time);
}
