#include "Engine/Core/Actor/Pawn/Character/Character.h"

void Character::update_state(float delta_time) {
	// 状態の更新
	state_.update(delta_time);
	// 状態タイマーの更新
	state_timer_ += delta_time;
}

#ifdef _DEBUG
void Character::debug_update(float delta_time) {
    // 名前
    std::string text = "name: " + name_;
    ImGui::Text(text.c_str());
    // 座標
    ImGui::Text("position: X:%.3f Y:%.3f Z:%.3f", transform_.position().x, transform_.position().y, transform_.position().z);
    // ステート番号
    ImGui::Text("state: %d", (int)state_.get_current_state());
    // モーション番号
    ImGui::Text("motion: %d", (int)motion_);
}
#endif  

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

float Character::state_timer() const {
    return state_timer_;
}

GSuint Character::current_state_num() const {
	return state_.get_current_state();
}

