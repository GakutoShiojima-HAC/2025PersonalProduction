// -----------------------------------------------------------------------------------------
//  File          : AttackColliderPool.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/08/12
//  Description   : �U������̃v�[��
//					����������̓v�[�����ōs���Ă��邽�ߕs�v
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ATTACK_COLLIDER_POOL_H_
#define ATTACK_COLLIDER_POOL_H_

#include "Engine/Utils/ObjectPool.h"
#include <list>

using namespace std;

class World;
class AttackCollider;

class AttackColliderPool {
public:
	AttackColliderPool(World* world);

	~AttackColliderPool();

public:
	void remove();

	void clear();

public:
	/// <summary>
	/// �U������𐶐�
	/// </summary>
	/// <param name="radius">= ����̔��a</param>
	/// <param name="center">= �������苅�̒��S</param>
	/// <param name="owner">= ������</param>
	/// <param name="damage">= �_���[�W�l</param>
	/// <param name="lifespan">= ����̎���</param>
	/// <param name="delay">= ���肪�L���ɂȂ�܂ł̒x��</param>
	void generate(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan = 1.0f, float delay = 0.0f);

private:
	World* world_{ nullptr };

	// �R���C�_�[�v�[��
	ObjectPool<AttackCollider> colliders_;
	// �A�N�e�B�u�ȃR���C�_�[���X�g
	list<AttackCollider*> active_colliders_;

public:
	// �R�s�[�֎~
	AttackColliderPool(const AttackColliderPool& other) = delete;
	AttackColliderPool& operator = (const AttackColliderPool& other) = delete;

};

#endif
