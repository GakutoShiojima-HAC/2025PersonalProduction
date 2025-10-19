#include "State/Player/PlayerInteractState.h"
#include "Actor/Player/Player.h"

PlayerInteractState::PlayerInteractState(Player& owner) :
	PlayerState(owner) {

}

void PlayerInteractState::update(float delta_time) {
    // カメラ切り替えの更新
    owner_.update_lockon_camera();
    // 移動の更新
    owner_.update_move(delta_time);

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
        owner_.attack_start();
        owner_.change_state((GSuint)PlayerStateType::Attack, owner_.get_attack_motion(), false);
        return;
    }

    // ジャンプ入力があればジャンプステートへ遷移
    if (owner_.is_action(InputAction::GAME_Jump)) {
        owner_.on_jump();
        owner_.change_state((GSuint)PlayerStateType::Jump, (GSuint)PlayerMotion::Jump, false);
        return;
    }

    // TODO インタラクトできるならインタラクトすること
    /*if (owner_.is_interact()) {
        owner_.change_state((GSuint)PlayerStateType::Interact, owner_.get_current_motion(), false);
        owner_.on_interact();
        return;
    }*/

    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}
