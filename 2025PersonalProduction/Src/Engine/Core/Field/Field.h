// -----------------------------------------------------------------------------------------
//  File        : Field.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/10
//  Updated     : 2025/07/10
//  Description : ���[���h�̃t�B�[���h�Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>
#include "Engine/Core/Actor/FieldActor/FieldActorManager.h"

class Ray;
class Line;
class BoundingSphere;

class Field {
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="octree">= �I�N�c���[�ԍ�</param>
	/// <param name="collider">= �Փ˔���p�I�N�c���[�ԍ�</param>
	/// <param name="skybox">= �X�J�C�{�b�N�X�ԍ�</param>
	Field(GSuint octree, GSuint collider, GSuint skybox);

	~Field();

public:
	void update(float delta_time);

	void draw() const;

	void clear();

public:
	/// <summary>
	/// �t�B�[���h�A�N�^�[��`��
	/// </summary>
	void draw_field_actor() const;

	/// <summary>
	/// �t�B�[���h�A�N�^�[�̒ǉ�
	/// </summary>
	/// <param name="actor">�t�B�[���h�A�N�^�[</param>
	void add(FieldActor* field_actor);
	
	/// <summary>
	/// ���C�Ƃ̏Փ˔���
	/// </summary>
	/// <param name="ray">= ���C</param>
	/// <param name="max_distance">= ���C�̒���</param>
	/// <param name="intersect">= �Փ˂�����_�̎���</param>
	/// <param name="plane">= �Փ˂����v���[���̎���</param>
	/// <param name="actor">= �Փ˂����t�B�[���h�A�N�^�[�̎���</param>
	/// <returns>�Փ˂��Ă�����^</returns>
	bool collide(const Ray& ray, float max_distance, GSvector3* intersect = nullptr, GSplane* plane = nullptr, Actor** actor = nullptr) const;

	/// <summary>
	/// �����Ƃ̏Փ˔���
	/// </summary>
	/// <param name="line">= ����</param>
	/// <param name="intersect">= �Փ˂�����_�̎���</param>
	/// <param name="plane">= �Փ˂����v���[���̎���</param>
	/// <param name="actor">= �Փ˂����t�B�[���h�A�N�^�[�̎���</param>
	/// <returns>�Փ˂��Ă�����^</returns>
	bool collide(const Line& line, GSvector3* intersect = nullptr, GSplane* plane = nullptr, Actor** actor = nullptr) const;

	/// <summary>
	/// ���̂Ƃ̏Փ˔���
	/// </summary>
	/// <param name="sphere">= ����</param>
	/// <param name="center">= �Փ˂������S���W�̎���</param>
	/// <param name="actor">= �Փ˂����t�B�[���h�A�N�^�[�̎���</param>
	/// <returns>�Փ˂��Ă�����^</returns>
	bool collide(const BoundingSphere& sphere, GSvector3* center = nullptr, Actor** actor = nullptr) const;

public:
	/// <summary>
	/// �`��p�I�N�c���[���擾
	/// </summary>
	GSuint& octree();

	/// <summary>
	/// �Փ˔���p�I�N�c���[���擾
	/// </summary>
	GSuint& collider();

	/// <summary>
	/// �X�J�C�{�b�N�X���擾
	/// </summary>
	GSuint& skybox();

private:
	// �I�N�c���[
	GSuint octree_{ 0 };
	// �Փ˔���I�N�c���[
	GSuint collider_{ 0 };
	// �X�J�C�{�b�N�X
	GSuint skybox_{ 0 };

	// �t�B�[���h�A�N�^�[
	FieldActorManager actors_;

#ifdef _DEBUG
	// �I�N�c���[��`�悷�邩�ǂ���
	bool draw_octree_{ true };
	// �X�J�C�{�b�N�X��`�悷�邩�ǂ���
	bool draw_skybox_{ true };
#endif

public:
	// �R�s�[�֎~
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

};

#endif
