#include "State/Player/PlayerAttackState.h"
#include "Actor/Player/Player.h"

PlayerAttackState::PlayerAttackState(Player& owner) :
	PlayerState(owner) {

}

void PlayerAttackState::enter() {
	next_attack_input_ = false;

	enter_next_attack_min_timer_ = owner_.get_enter_next_attack_min_time();
    enter_next_attack_max_timer_ = owner_.get_enter_next_attack_max_time();
}

void PlayerAttackState::update(float delta_time) {
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

    // 次の段に進む入力を検知 (/1,5fは少し早めに押せるようにしてある)
    if (
        owner_.state_timer() >= enter_next_attack_min_timer_ / 1.5f &&
        owner_.state_timer() <= enter_next_attack_max_timer_ &&
        owner_.is_action(InputAction::GAME_Attack)
        ) next_attack_input_ = true;

    // 次の段に進める時間かつ、入力が既にあれば
	if (owner_.state_timer() >= enter_next_attack_min_timer_ && next_attack_input_) {
        owner_.look_target();
        // 次もこのステートなので値をリセット
		next_attack_input_ = false;
        // 段を加算(is_action内で次に進めるときのみ真を返すためチェック不要)
		owner_.attack_count() += 1;
        // 次の段に進める時間を更新
        enter_next_attack_min_timer_ = owner_.get_enter_next_attack_min_time();
        enter_next_attack_max_timer_ = owner_.get_enter_next_attack_max_time();
        // モーション更新のために呼び出す
		owner_.change_state((GSuint)PlayerStateType::Attack, owner_.get_attack_motion(), false);
		return;
	}

    // 次の段に進めないかつ、移動入力があれば移動ステートに遷移
    if (owner_.state_timer() > enter_next_attack_max_timer_ && owner_.is_move_input()) {
        owner_.to_move_state();
        return;
    }

    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerAttackState::exit() {
	owner_.attack_count() = 0;
}
