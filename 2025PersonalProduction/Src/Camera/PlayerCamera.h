// -----------------------------------------------------------------------------------------
//  File          : PlayerCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/11
//  Updated       : 2025/08/11
//  Description   : �v���C���[�J����
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_CAMERA_H_
#define PLAYER_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Input/Input.h"

class Pawn;

class PlayerCamera : public Camera {
public:
	PlayerCamera(IWorld* world);

public:
	void update(float delta_time) override;

	void die() override;

public:
	/// <summary>
	/// ��_�Ƃ���|�[����ݒ�
	/// </summary>
	/// <param name="owner">= ��_</param>
	void set_owner(Pawn* owner);

	/// <summary>
	/// ���b�N�I���^�[�Q�b�g�Ƃ���|�[����ݒ�
	/// </summary>
	/// <param name="target">= �^�[�Q�b�g</param>
	void set_lockon_target(Pawn* target);

	/// <summary>
	/// ���b�N�I�������ǂ���
	/// </summary>
	/// <returns>���Ă�����^��ԋp</returns>
	bool is_lockon() const;

	/// <summary>
	/// ���b�N�I�����̃^�[�Q�b�g��ԋp
	/// </summary>
	/// <returns>���Ă�����|�[�����A���Ă��Ȃ����nullptr</returns>
	Pawn* get_lockon_target();

private:
	Input& input_ = Input::get_instance();

	// �J�����̊�_�Ƃ���|�[��
	Pawn* owner_{ nullptr };
	// ���b�N�I���^�[�Q�b�g�Ƃ���|�[��
	Pawn* lockon_target_{ nullptr };

	// y������̉�]�p�x
	float yaw_{ 0.0f };
	// x������̉�]�p�x
	float pitch_{ 0.0f };

};
#endif
