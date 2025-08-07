#include "Engine/Core/Actor/Pawn/Character/Character.h"

void Character::change_state_request(const GSuint state_num) {

}

void Character::update_state(float delta_time) {
	// ��Ԃ̍X�V
	state_.update(delta_time);
	// ��ԃ^�C�}�[�̍X�V
	state_timer_ += delta_time;
}

void Character::change_state(const GSuint state_num) {
	// ��Ԃ̕ύX
	state_.change_state(state_num);
	// ��ԃ^�C�}�[�̏�����
	state_timer_ = 0.0f;
}

void Character::change_state(const GSuint state_num, const GSuint motion_num, const bool loop) {
	change_state(state_num);

	// ���[�V�����ԍ��ύX
	motion_ = motion_num;
	// ���[�V�����̃��[�v�w��
	motion_loop_ = loop;
	// ���[�V�����̕ύX
	mesh_.change_motion(motion_num, loop);
}

