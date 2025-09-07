
// -----------------------------------------------------------------------------------------
//  File          : DummyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/09/07
//  Description   : ダミーエネミー 全てのデバッグ機能を持つようにしたい
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
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
	/// 経路探索の移動が終了したかどうか
	/// </summary>
	/// <returns>終了していたら真を返却</returns>
	bool is_move_end() const;

	/// <summary>
	/// 経路に沿って移動する
	/// </summary>
	void update_move(float delta_time);

	/// <summary>
	/// 攻撃判定を生成するかどうか
	/// </summary>
	bool is_generate_collider() const;

private:
	/// <summary>
	/// 攻撃判定を生成
	/// </summary>
	void generate_attack_collider();

	/// <summary>
	/// 経路に沿って移動を開始する
	/// </summary>
	void move_start();

private:
	// 攻撃判定を生成するか
	bool is_generate_collider_{ false };
	// 攻撃判定のオフセット
	GSvector3 generate_offset_{ 0.0f, 1.0f, 1.0f };
	// 攻撃判定の半径
	float collider_radius_{ 0.5f };
	// 攻撃判定のダメージ
	int collider_damage_{ 1 };

	// ナビメッシュ
	NavMeshAgent navmesh_{};
	// 移動先
	GSvector3 move_to_{ -1.0f, 0.5f, 0.0f };

};

using DummyEnemyMotion = DummyEnemy::Motion;

#endif