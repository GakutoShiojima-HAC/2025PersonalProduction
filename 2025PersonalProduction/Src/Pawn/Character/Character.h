// -----------------------------------------------------------------------------------------
//  File          : Character.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : �X�e�[�g�}�V�[�������|�[���̐e
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Pawn/Pawn.h"

class Character : public Pawn {
public:
	Character() = default;

	virtual ~Character() = default;

public:
	/// <summary>
	/// �X�e�[�g�̕ύX�����N�G�X�g
	/// </summary>
	/// <param name="state_num">= �X�e�[�g�ԍ�</param>
	virtual void change_state_request(const GSuint state_num);

protected:
	/// <summary>
	/// �X�e�[�g�}�V�[���̍X�V
	/// </summary>
	void update_state(float delta_time);

	/// <summary>
	/// �X�e�[�g�̕ύX
	/// </summary>
	/// <param name="state_num">= �X�e�[�g�ԍ�</param>
	void change_state(const GSuint state_num);

	/// <summary>
	/// �X�e�[�g�̕ύX(���[�V�����w�肠��)
	/// </summary>
	/// <param name="state_num">= �X�e�[�g�ԍ�</param>
	/// <param name="motion_num">= ���[�V�����ԍ�</param>
	/// <param name="loop">= ���[�v���邩�ǂ���</param>
	void change_state(const GSuint state_num, const GSuint motion_num, const bool loop);

protected:
	// �X�e�[�g�}�V��
	StateMachine state_;
	// ��ԃ^�C�}�[
	float state_timer_{ 0.0f };

};

#endif
