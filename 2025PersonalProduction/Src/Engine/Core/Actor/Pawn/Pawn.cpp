#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Utils/Line.h"
#include "GameConfig.h"

void Pawn::take_damage(Actor& other, const int damage) {
	hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
}

void Pawn::on_jump() {
	velocity_.y = jump_power_ * 0.1f + gravity_ * 0.1f / cFPS;	// �d�͂����Z���邱�Ƃŏ������ێ�
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
	velocity_.y -= gravity_ * 0.1f / cFPS * delta_time;
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
	// x,z���̉�]�𖳌��ɂ���
	transform_.rotation(GSquaternion(0.0f, transform_.rotation().y, 0.0f, transform_.rotation().w));

	/* �ǂƂ̏Փ˔���i���̂Ƃ̔���j */
	GSvector3 center; // �����߂���̋��̂̒��S���W
	if (world_->get_field()->collide(collider(), &center)) {
		// y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		// �␳��̍��W�ɕύX����
		transform_.position(center);
	}
	
	/* �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j*/
	// �n�ʂƂ̌�_
	GSvector3 intersect;
	// �Փ˂����t�B�[���h�p�A�N�^�[
	Actor* field_actor{ nullptr };
	// �e�����Z�b�g���Ă���
	transform_.parent(nullptr);

	// ������W
	GSvector3 position_head = transform_.position();
	GSvector3 position_foot = transform_.position();
	Line head_line;
	head_line.start = position_head + collider_.center;
	head_line.end = position_head + GSvector3{ 0.0f, head_offset_, 0.0f };
	Line foot_line;
	foot_line.start = position_foot + collider_.center;
	foot_line.end = position_foot + GSvector3{ 0.0f, -foot_offset_, 0.0f };

	// �V�䔻��
	if (world_->get_field()->collide(head_line, &intersect, nullptr, &field_actor)) {
		// TODO intersect��y��������

		// ���W��ύX����
		transform_.position(position_head);
		// �d�͂�����������
		velocity_.y = 0.0f;
	}

	// �n�ʔ���
	if (world_->get_field()->collide(foot_line, &intersect, nullptr, &field_actor)) {
		// ���������_����y���W�̂ݕ␳����
		position_foot.y = intersect.y;
		// ���W��ύX����
		transform_.position(position_foot);
		// �d�͂�����������
		velocity_.y = 0.0f;
		// �t�B�[���h�p�̃A�N�^�[�N���X�ƏՓ˂�����
		if (field_actor != nullptr) {
			// �Փ˂����t�B�[���h�p�̃A�N�^�[��e�̃g�����X�t�H�[���N���X�Ƃ��Đݒ�
			transform_.parent(&field_actor->transform());
		}
		// ���n��Ԃ̍X�V
		on_ground();
		is_ground_ = true;
	}
	else {
		// �󒆏�Ԃ̍X�V
		on_air();
		is_ground_ = false;
	}
}

void Pawn::collide_actor(Actor& other) {
	// ���g�ƑΏۂ̍��W���擾
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// ���������߂�
	float distance = GSvector3::distance(position, target);

	// �Փ˔��苅�̔��a���m���������������擾
	float length = collider_.radius + other.collider().radius;

	// �Փ˔���̏d�Ȃ��Ă��钷���̎擾
	float overlap = length - distance;

	// �d�Ȃ��Ă��镔���̔����̋������������
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);

	// �t�B�[���h�Ƃ̏Փ˔�����ēx�s��
	collide_field();
}
