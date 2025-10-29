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

    // スキル入力があればスキルステートへ遷移
    if (owner_.is_action(InputAction::GAME_Skill)) {
        owner_.on_skill();
        owner_.change_state((GSuint)PlayerStateType::Skill, owner_.get_skill_motion(), false);
        return;
    }

    // 攻撃入力があれば攻撃ステートへ遷移
    if (owner_.is_action(InputAction::GAME_Attack)) {
        owner_.look_target();
        owner_.attack_start();
        owner_.change_state((GSuint)PlayerStateType::Attack, owner_.get_attack_motion(), false);
        return;
    }

    // ジャンプ入力があればジャンプステートへ遷移
    if (owner_.is_action(InputAction::GAME_Jump)) {
        owner_.reset_move_speed();
        owner_.on_jump();
        owner_.change_state((GSuint)PlayerStateType::Jump, (GSuint)PlayerMotion::Jump, false);
        return;
    }

    // 移動入力があれば移動ステートに遷移
    if (owner_.is_move_input()) {
        owner_.to_move_state();
        return;
    }

    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}
