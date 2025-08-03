// -----------------------------------------------------------------------------------------
//  File        : Camera.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/10
//  Updated     : 2025/07/10
//  Description : �Q�[�����ɓo�ꂷ��J�����̐e�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Camera/CameraTag.h"
#include <GStransform.h>

class IWorld;

class Camera {
public:
	Camera() = default;

	virtual ~Camera() = default;

public:
	virtual void update(float delta_time);

	/// <summary>
	/// ���̃J�����ɐ؂�ւ�������̏���
	/// </summary>
	virtual void enter();

	/// <summary>
	/// �ʂ̃J�����ɐ؂�ւ�������̏���
	/// </summary>
	virtual void exit();

	/// <summary>
	/// �J�����̎������I��点��
	/// </summary>
	virtual void die();

public:
	/// <summary>
	/// �J�����^�O���擾
	/// </summary>
	/// <returns>�J�����^�O</returns>
	CameraTag& tag();

	/// <summary>
	/// �g�����X�t�H�[����ԋp(const)
	/// </summary>
	const GStransform& transform() const;

	/// <summary>
	/// �g�����X�t�H�[����ԋp
	/// </summary>
	GStransform& transform();

	/// <summary>
	/// �J�����̎������s�������ǂ���
	/// </summary>
	bool is_dead() const;

	/// <summary>
	/// �J�������g���Ă��邩�ǂ���
	/// </summary>
	bool& is_using();

protected:
	// ���[���h
	IWorld* world_{ nullptr };
	// ����
	bool is_dead_{ false };
	// �g�p�����ǂ���
	bool is_using_{ false };
	// �J�����^�O
	CameraTag tag_{ CameraTag::None };
	// �g�����X�t�H�[��
	GStransform transform_;

public:
	// �R�s�[�֎~
	Camera(const Camera& other) = delete;
	Camera& operator = (const Camera& other) = delete;

};

#endif
