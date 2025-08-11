
// -----------------------------------------------------------------------------------------
//  File          : DummyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/08/12
//  Description   : �_�~�[�G�l�~�[
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef DUMMY_ENEMY_H_
#define DUMMY_ENEMY_H_

#include "Engine/Core/Actor/Pawn/Pawn.h"

class DummyEnemy : public Pawn {
public:
	DummyEnemy(IWorld* world, const GSvector3& position);

public:
	void update(float delta_time) override;

	void draw() const override;

public:
	void take_damage(Actor& other, const int damage) override;

private:
	void react(Actor& other) override;

private:
	/// <summary>
	/// �U������̐������X�V
	/// </summary>
	void update_generate_collider(float delta_time);

private:
	// �U������𐶐����邩
	bool is_generate_collider_{ false };
	// �U������̐����Ԋu
	float generate_interval_{ 1.0f };
	// �U������̐����^�C�}�[
	float generate_timer_{ 0.0f };
	// �U������̃I�t�Z�b�g
	GSvector3 generate_offset_{ 0.0f, 1.0f, 1.0f };
	// �U������̔��a
	float collider_radius_{ 0.5f };
	// �U������̃_���[�W
	int collider_damage_{ 1 };

};

#endif