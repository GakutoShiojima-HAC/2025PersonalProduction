#include "State/Player/PlayerAvoidState.h"
#include "Actor/Player/Player.h"

constexpr float AVOID_DISABLE_TIME{ 25.0f };
constexpr float MOTION_CANCEL_TIME{ 40.0f };

PlayerAvoidState::PlayerAvoidState(Player& owner) :
	PlayerState(owner) {

}

void PlayerAvoidState::enter() {
    avoid_attack_input_ = false;
    avoid_skill_input_ = false;

    owner_.set_draw_weapon(false);
}

void PlayerAvoidState::update(float delta_time) {
    const float motion_time = owner_.currnent_motion_time();

    if (motion_time > AVOID_DISABLE_TIME) {
        // 回避判定を削除
        owner_.disable_avoid_collider();
    }

    if (motion_time > MOTION_CANCEL_TIME) {
        // スキル入力があれば特殊スキルステートへ遷移
        if (avoid_skill_input_) {
            // TODO
        }

        // 攻撃入力があれば特殊攻撃ステートへ遷移
        if (avoid_attack_input_) {
            owner_.on_avoid_attack();
            return;
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

    // 回避からつながるコンボアクション入力の検知
    if (owner_.currnent_motion_time() < MOTION_CANCEL_TIME) {
        if (owner_.is_action(InputAction::GAME_Attack)) avoid_attack_input_ = true;
        if (owner_.is_action(InputAction::GAME_Skill)) avoid_skill_input_ = true;
    }
        
    // モーションが終了したら移動ステートに遷移
	if (owner_.is_motion_end()) {
		owner_.to_move_state();
		return;
	}
}

void PlayerAvoidState::exit() {
    owner_.disable_avoid_collider();
}

