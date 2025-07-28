// -----------------------------------------------------------------------------------------
//  File          : Actor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �A�N�^�[�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ACTOR_H_
#define ACTOR_H_

#include <GStransform.h>
#include <string>
#include "Actor/ActorTag.h"
#include "Engine/Core/Collision/BoundingSphere.h"
#include "GameConfig.h"

using namespace std;

class IWorld;

class Actor {
public:
	Actor() = default;

	virtual ~Actor() = default;

public:
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void update(float delta_time);

	/// <summary>
	/// �X�V update��collide�̌�ɌĂ΂��
	/// </summary>
	virtual void late_update(float delta_time);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void draw() const;

	/// <summary>
	/// �������`��
	/// </summary>
	virtual void draw_transparent() const;

	/// <summary>
	/// GUI�`��
	/// </summary>
	virtual void draw_gui() const;

	/// <summary>
	/// �A�N�^�[�̎������I��点��
	/// </summary>
	virtual void die();

	/// <summary>
	/// ���A�N�^�[�ƏՓ˂����Ƃ��ɌĂ΂��Փˏ����Փˏ���
	/// </summary>
	/// <param name="other">= �Փ˂����A�N�^�[</param>
	virtual void react(Actor& other);

public:
	/// <summary>
	/// �Փ˔���
	/// </summary>
	void collide(Actor& other);

	/// <summary>
	/// �A�N�^�[�̎������s�������ǂ���
	/// </summary>
	bool is_dead() const;

	/// <summary>
	/// �A�N�^�[�̎�����0�������ꍇ��������������܂ōs�����ǂ���
	/// </summary>
	/// <returns></returns>
	bool is_clear() const;

	/// <summary>
	/// �Փ˂��Ă��邩�ǂ�����ԋp
	/// </summary>
	bool is_collide(const Actor& other) const;

	/// <summary>
	/// �A�N�^�[�̖��O��ԋp
	/// </summary>
	string& name();

	/// <summary>
	/// �A�N�^�[�̃^�O��ԋp
	/// </summary>
	ActorTag& tag();

	/// <summary>
	/// �g�����X�t�H�[����ԋp(const)
	/// </summary>
	const GStransform& transform() const;

	/// <summary>
	/// �g�����X�t�H�[����ԋp
	/// </summary>
	GStransform& transform();

	/// <summary>
	/// �ړ��ʂ�ԋp
	/// </summary>
	GSvector3 velocity() const;

	/// <summary>
	/// �Փ˔���f�[�^���擾
	/// </summary>
	BoundingSphere collider() const;

	/// <summary>
	/// ���[�J���ړ�������̃��[���h��ԍs���ԋp
	/// </summary>
	/// <param name="position">= �ړ���</param>
	/// <param name="rotate">= ��]��</param>
	/// <param name="scale">= �g�k��</param>
	GSmatrix4 local_to_world(const GSvector3& position, const GSvector3& rotate, const GSvector3& scale) const;

protected:
	/// <summary>
	/// �G�t�F�N�V�A�̃G�t�F�N�g���Đ�
	/// </summary>
	/// <param name="effect_id">= �G�t�F�N�g�n���h��</param>
	/// <param name="position">= ���[�J���ړ���</param>
	/// <param name="rotation">= ���[�J����]��</param>
	/// <param name="scale">= ���[�J���g�k��</param>
	GSuint play_effect(GSuint effect_id, const GSvector3& position, const GSvector3& rotate = GSvector3{ 0.0f, 0.0f, 0.0f }, const GSvector3& scale = GSvector3{ 1.0f, 1.0f, 1.0f }) const;

protected:
	// ���[���h
	IWorld* world_{ nullptr };
	
	// ����
	bool is_dead_{ false };
	// �������s�����Ƃ�����������܂ōs����
	// ***�댯*** �I�u�W�F�N�g�v�[���ł̂ݎg�p���邱��
	bool is_clear_{ true };	

	// ���O
	string name_{ "Actor" };
	// �^�O
	ActorTag tag_{ ActorTag::None };

	// �g�����X�t�H�[��
	GStransform transform_;
	// �ړ���
	GSvector3 velocity_{ 0.0f, 0.0f, 0.0f };

	// �Փ˔�����s�����ǂ���
	bool enable_collider_{ true };
	// ���̏Փ˔���
	BoundingSphere collider_;

public:
	// �R�s�[�֎~
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

};

#endif
