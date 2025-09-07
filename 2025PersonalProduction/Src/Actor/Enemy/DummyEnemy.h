
// -----------------------------------------------------------------------------------------
//  File          : DummyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/09/07
//  Description   : �_�~�[�G�l�~�[ �S�Ẵf�o�b�O�@�\�����悤�ɂ�����
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef DUMMY_ENEMY_H_
#define DUMMY_ENEMY_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"

class DummyEnemy : public Character {
public:
	DummyEnemy(IWorld* world, const GSvector3& position);

public:
	enum Motion {
		Idle = 2,
		Move = 3,
		Attack = 0,
		Dead = 1
	};

public:
	void update(float delta_time) override;

	void draw() const override;

public:
	void take_damage(Actor& other, const int damage) override;

	bool is_dead_state() const override;

private:
	void react(Actor& other) override;

	void add_state() override;

public:
	/// <summary>
	/// �o�H�T���̈ړ����I���������ǂ���
	/// </summary>
	/// <returns>�I�����Ă�����^��ԋp</returns>
	bool is_move_end() const;

	/// <summary>
	/// �o�H�ɉ����Ĉړ�����
	/// </summary>
	void update_move(float delta_time);

	/// <summary>
	/// �U������𐶐����邩�ǂ���
	/// </summary>
	bool is_generate_collider() const;

private:
	/// <summary>
	/// �U������𐶐�
	/// </summary>
	void generate_attack_collider();

	/// <summary>
	/// �o�H�ɉ����Ĉړ����J�n����
	/// </summary>
	void move_start();

private:
	// �U������𐶐����邩
	bool is_generate_collider_{ false };
	// �U������̃I�t�Z�b�g
	GSvector3 generate_offset_{ 0.0f, 1.0f, 1.0f };
	// �U������̔��a
	float collider_radius_{ 0.5f };
	// �U������̃_���[�W
	int collider_damage_{ 1 };

	// �i�r���b�V��
	NavMeshAgent navmesh_{};
	// �ړ���
	GSvector3 move_to_{ -1.0f, 0.5f, 0.0f };

};

using DummyEnemyMotion = DummyEnemy::Motion;

#endif