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
	// �d�͂�������
	velocity_.y -= gravity_ * delta_time;
	// �d�͂𔽉f
	transform_.translate(0.0f, velocity_.y, 0.0f);
}

void Pawn::update_invincible(float delta_time) {
	// �Փ˔��肪�L���Ŗ��G���Ԃ�������ΏI��
	if (enable_collider_ && invincible_timer_ <= 0.0f) return;

	// ���G���Ԃ����炷
	invincible_timer_ = CLAMP(invincible_timer_ - delta_time / cFPS, 0.0f, FLT_MAX);
	// ���G���Ԃ������Ȃ�����Փ˔���𕜊�
	if (invincible_timer_ <= 0.0f && !enable_collider_) enable_collider_ = true;
}

void Pawn::update_mesh(float delta_time) {
	// ���b�V���̃��[�V�������X�V
	mesh_.update(delta_time);
	// ���[���h�ϊ��s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
}

void Pawn::collide_field() {
	// TODO 
}
