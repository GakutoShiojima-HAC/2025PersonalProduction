#include "State/Player/PlayerMoveState.h"
#include "Actor/Player/Player.h"

PlayerMoveState::PlayerMoveState(Player& owner) :
	PlayerState(owner) {

}

void PlayerMoveState::enter() {
    owner_.set_draw_weapon(false);
}

void PlayerMoveState::update(float delta_time) {
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
        owner_.look_target();
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
}

void PlayerMoveState::draw() const {

}

void PlayerMoveState::draw_gui() const {

}

void PlayerMoveState::exit() {

}
