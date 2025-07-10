#include "Pawn/Pawn.h"

void Pawn::take_damage(Actor& other, const int damage) {
	hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
}

int& Pawn::hp() {
	return hp_;
}

float& Pawn::gravity() {
	return gravity_;
}

float& Pawn::invincible_timer() {
	return invincible_timer_;
}

void Pawn::update_gravity(float delta_time) {
	// 重力を加える
	velocity_.y -= gravity_ * delta_time;
	// 重力を反映
	transform_.translate(0.0f, velocity_.y, 0.0f);
}

void Pawn::update_invincible(float delta_time) {
	// 衝突判定が有効で無敵時間が無ければ終了
	if (enable_collider_ && invincible_timer_ <= 0.0f) return;

	// 無敵時間を減らす
	invincible_timer_ = CLAMP(invincible_timer_ - delta_time / cFPS, 0.0f, FLT_MAX);
	// 無敵時間が無くなったら衝突判定を復活
	if (invincible_timer_ <= 0.0f && !enable_collider_) enable_collider_ = true;
}

void Pawn::update_mesh(float delta_time) {
	// メッシュのモーションを更新
	mesh_.update(delta_time);
	// ワールド変換行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

void Pawn::collide_field() {
	// TODO 
}
