
// -----------------------------------------------------------------------------------------
//  File          : DummyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/08/12
//  Description   : ダミーエネミー
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
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
	/// 攻撃判定の生成を更新
	/// </summary>
	void update_generate_collider(float delta_time);

private:
	// 攻撃判定を生成するか
	bool is_generate_collider_{ false };
	// 攻撃判定の生成間隔
	float generate_interval_{ 1.0f };
	// 攻撃判定の生成タイマー
	float generate_timer_{ 0.0f };
	// 攻撃判定のオフセット
	GSvector3 generate_offset_{ 0.0f, 1.0f, 1.0f };
	// 攻撃判定の半径
	float collider_radius_{ 0.5f };
	// 攻撃判定のダメージ
	int collider_damage_{ 1 };

};

#endif