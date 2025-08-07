// -----------------------------------------------------------------------------------------
//  File          : Pawn.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : Actor���p������A�ӎv��������
//					�L�����N�^�[�A�G�[�W�F���g���p������
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PAWN_H_
#define PAWN_H_

#include "Engine/Core/Actor/Actor.h"
#include "Engine/Graphics/AnimatedMesh/AnimatedMesh.h"

class Pawn : public Actor {
public:
	Pawn() = default;

	virtual ~Pawn() = default;

public:
	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="other">= �^���鑤�̃|�[��</param>
	/// <param name="damage">= �_���[�W�l</param>
	virtual void take_damage(Actor& other, const int damage);

public:
	/// <summary>
	/// HP���擾
	/// </summary>
	int& hp();

	/// <summary>
	/// �d�͒l���擾
	/// </summary>
	float& gravity();

	/// <summary>
	/// ���G���Ԃ��擾
	/// </summary>
	float& invincible_timer();

protected:
	/// <summary>
	/// �d�͂̍X�V
	/// </summary>
	void update_gravity(float delta_time);

	/// <summary>
	/// ���G���Ԃ̍X�V
	/// </summary>
	void update_invincible(float delta_time);

	/// <summary>
	/// ���b�V���̍X�V
	/// </summary>
	void update_mesh(float delta_time);

	/// <summary>
	/// �n�`�Ƃ̏Փ˔���
	/// </summary>
	virtual void collide_field();

	/// <summary>
	/// �A�N�^�[�Ƃ̏Փ˔���
	/// </summary>
	virtual void collide_actor(Actor& other);

protected:
	// �A�j���[�V�������b�V��
	AnimatedMesh mesh_{ 0, 0, 0, 0 };
	// ���[�V�����ԍ�
	GSuint motion_{ 0 };
	// ���[�V�����̃��[�v�w��
	bool motion_loop_{ true };

	// HP
	int hp_{ 1 };
	// �d�͒l
	float gravity_{ 0 };
	// ���G���ԃ^�C�}�[
	float invincible_timer_{ 0 };

};

#endif
