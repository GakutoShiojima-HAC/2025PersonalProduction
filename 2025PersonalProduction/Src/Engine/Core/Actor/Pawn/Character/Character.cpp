#include "Engine/Core/Actor/Pawn/Character/Character.h"

void Character::change_state_request(const GSuint state_num) {

}

void Character::update_state(float delta_time) {
	// 状態の更新
	state_.update(delta_time);
	// 状態タイマーの更新
	state_timer_ += delta_time;
}

void Character::change_state(const GSuint state_num) {
	// 状態の変更
	state_.change_state(state_num);
	// 状態タイマーの初期化
	state_timer_ = 0.0f;
}

void Character::change_state(const GSuint state_num, const GSuint motion_num, const bool loop) {
	change_state(state_num);

	// モーション番号変更
	motion_ = motion_num;
	// モーションのループ指定
	motion_loop_ = loop;
	// モーションの変更
	mesh_.change_motion(motion_num, loop);
}

