// -----------------------------------------------------------------------------------------
//  File          : AttackCollider.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/08/12
//  Description   : �U������
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ATTACK_COLLIDER_H_
#define ATTACK_COLLIDER_H_

#include "Engine/Core/Actor/Actor.h"

class AttackCollider : public Actor {
public:
	AttackCollider(float radius = 0.0f, const GSvector3& center = GSvector3{ 0.0f, 0.0f, 0.0f }, Actor* owner = nullptr, int damage = 0, float lifespan = 1.0f, float delay = 0.0f);

public:
	void update(float delta_time) override;

	void draw() const override;

	void react(Actor& other) override;

public:
	/// <summary>
	/// �ēx�g����悤�ɂ���
	/// </summary>
	/// <param name="radius">= ����̔��a</param>
	/// <param name="center">= �������苅�̒��S</param>
	/// <param name="owner">= ������</param>
	/// <param name="damage">= �_���[�W�l</param>
	/// <param name="lifespan">= ����̎���</param>
	/// <param name="delay">= ���肪�L���ɂȂ�܂ł̒x��</param>
	/// <param name="disable_clear">= �}�l�[�W���[����̃���������Ώۂ��珜�O���邩�ǂ���</param>
	void reactivate(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan = 1.0f, float delay = 0.0f, bool disable_clear = false);

private:
	Actor* owner_{ nullptr };

	// ����
	float lifespan_timer_{ 1.0f };
	// �����x��
	float delay_timer_{ 0.0f };

	// �_���[�W
	int damage_{ 0 };
};

#endif
