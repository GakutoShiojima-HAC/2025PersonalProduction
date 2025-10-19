#include "State/Player/PlayerLandState.h"
#include "Actor/Player/Player.h"

PlayerLandState::PlayerLandState(Player& owner) :
	PlayerState(owner) {

}

void PlayerLandState::update(float delta_time) {
    // 回避入力があれば回避ステートへ遷移
    if (owner_.is_action(InputAction::GAME_Avoid)) {
        owner_.on_avoid();
        owner_.change_state((GSuint)PlayerStateType::Avoid);
        return;
    }

    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}
