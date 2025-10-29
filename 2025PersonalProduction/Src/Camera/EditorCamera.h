// -----------------------------------------------------------------------------------------
//  File          : EditorCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/04
//  Updated       : 2025/08/04
//  Description   : �ҏW�p�J����
//					Active�ɂ�����ԂŉE�N���b�N�������Ă���Ԃ������������Ƃ��ł���
//					���̃J�������g���ăf�o�b�O��^�C�����C���̕ҏW���s��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef EDITOR_CAMERA_H_
#define EDITOR_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Input/Input.h"

class EditorCamera : public Camera {
public:
	EditorCamera(IWorld* world);

public:
	void update(float delta_time) override;

	void exit() override;

	void die() override;

private:
	/// <summary>
	/// �J�����^�C�����C�����Đ������ǂ���
	/// </summary>
	/// <returns>�Đ����Ȃ�^��ԋp</returns>
	bool is_play_timeline() const;

	/// <summary>
	/// �J�����̐؂�ւ�
	/// </summary>
	void toggle_camera();

private:
	Input& input_ = Input::get_instance();

	// ���̃J����
	Camera* prev_{ nullptr };

	// �G�f�B�^�J�������g���邩�ǂ���
	bool is_active_{ false };

	// y������̉�]�p�x
	float yaw_{ 0.0f };
	// x������̉�]�p�x
	float pitch_{ 0.0f };
	// �X��
	float angle_{ 0.0f };

};

#endif
