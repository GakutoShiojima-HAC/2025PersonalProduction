#include "State/Player/PlayerAvoidState.h"
#include "Actor/Player/Player.h"

PlayerAvoidState::PlayerAvoidState(Player& owner) :
	PlayerState(owner) {

}

void PlayerAvoidState::update(float delta_time) {
    // モーションキャンセル
    if (owner_.state_timer() > 58.0f) {
        // スキル入力があれば特殊スキルステートへ遷移
        if (owner_.is_action(InputAction::GAME_Skill)) {
            // TODO
        }

        // 攻撃入力があれば特殊攻撃ステートへ遷移
        if (owner_.is_action(InputAction::GAME_Attack)) {
            // TODO
        }

        // ジャンプ入力があればジャンプステートへ遷移
        if (owner_.is_action(InputAction::GAME_Jump)) {
            owner_.on_jump();
            owner_.change_state((GSuint)PlayerStateType::Jump, (GSuint)PlayerMotion::Jump, false);
            return;
        }

        // 移動入力があれば移動ステートに遷移
        if (owner_.is_move_input()) {
            owner_.to_move_state();
            return;
        }
    }
        
    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

