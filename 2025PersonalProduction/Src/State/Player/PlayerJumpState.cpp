#include "State/Player/PlayerJumpState.h"
#include "Actor/Player/Player.h"

PlayerJumpState::PlayerJumpState(Player& owner) :
	PlayerState(owner) {

}

void PlayerJumpState::update(float delta_time) {
    // 空中移動の更新
	owner_.update_move_air(delta_time);

    // モーションが終了したら移動ステートに遷移(落下状態への遷移は地形との衝突判定で行う)
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

