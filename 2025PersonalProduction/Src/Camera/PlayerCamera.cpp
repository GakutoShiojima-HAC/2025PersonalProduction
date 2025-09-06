#include "Camera/PlayerCamera.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Utils/Line.h"

// �����_(��_����̃I�t�Z�b�g)
const GSvector3 LOOKAT_ORIGIN_TO_OFFSET{ 0.0f, 1.75f, 0.0f };
// �����_����̈ʒu
const GSvector3 CAMERA_OFFSET{ 0.0f, 3.0f, -7.0f };
// ���_�ړ����x
const float SENSITIVITY_X{ 0.095f };
const float SENSITIVITY_Y{ 0.075f };

// �X���[�X�_���v��Ԏ���
const float SMOOTH_TIME{ 2.0f };
// �X���[�X�_���v�ړ���
const float SMOOTH_MAX_SPEED{ 5.0f };

PlayerCamera::PlayerCamera(IWorld* world) {
	world_ = world;
	tag_ = CameraTag::Player;
}

void PlayerCamera::update(float delta_time) {
	auto exclusion = [](Pawn*& pawn) {
		if (pawn == nullptr) return;
		if (pawn->is_dead_state() || pawn->is_dead()) pawn = nullptr;
	};
	exclusion(owner_);
	exclusion(lockon_target_);

	// �J�������W�̍X�V
	if (owner_ == nullptr) return;

	GSvector3 pos;
	GSvector3 at;
	if (is_lockon()) {
		// TODO
	}
	else {
		// ���_�ړ�
		GSvector2 input = input_.right_axis();
		if (input_.is_pad()) input *= 15.0f;
		yaw_ -= input.x * SENSITIVITY_X;
		pitch_ -= input.y * SENSITIVITY_Y;
		pitch_ = CLAMP(pitch_, -80.0f, 55.0f);

		// �����_�̍��W�����߂�
		at = owner_->transform().position() + LOOKAT_ORIGIN_TO_OFFSET;
		// �J�����̍��W�����߂�
		pos = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * CAMERA_OFFSET;	
	}

	// �J�������W���璍���_�Ԃɏ�Q�������邩�ǂ���
	Line line{ at, pos };
	GSvector3 intersect;
	if (world_->get_field()->collide(line, &intersect)) {
		// �ʒu��␳(����ł�������ǂ͓�����)
		pos = intersect;
	}

	// �X���[�X�_���v�ɂ�銊�炩�ȕ��
	GSvector3 tmp_velocity = GSvector3::zero();	// ���ړ���
	pos = GSvector3::smoothDamp(transform_.position(), pos, tmp_velocity, SMOOTH_TIME, SMOOTH_MAX_SPEED, delta_time);

	// ���W�̐ݒ�
	transform_.position(pos);
	// �����_�̕���������
	transform_.lookAt(at);
}

void PlayerCamera::die() {
	// �O�����玀�S����ɂł��Ȃ��悤�ɂ���
}

void PlayerCamera::set_owner(Pawn* owner) {
	owner_ = owner;
}

void PlayerCamera::set_lockon_target(Pawn* target) {
	lockon_target_ = target;
}

bool PlayerCamera::is_lockon() const {
	return lockon_target_ != nullptr;
}

Pawn* PlayerCamera::get_lockon_target() {
	return lockon_target_;
}
