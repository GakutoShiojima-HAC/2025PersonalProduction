// -----------------------------------------------------------------------------------------
//  File          : Pawn.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : Actorを継承する、意思を持つもの
//					キャラクター、エージェントが継承する
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PAWN_H_
#define PAWN_H_

#include "Engine/Core/Actor/Actor.h"
#include "Engine/Graphics/AnimatedMesh/AnimatedMesh.h"

class Pawn : public Actor {
public:
	Pawn() = default;

	virtual ~Pawn() = default;

public:
	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="other">= 与える側のポーン</param>
	/// <param name="damage">= ダメージ値</param>
	virtual void take_damage(Actor& other, const int damage);

public:
	/// <summary>
	/// HPを取得
	/// </summary>
	int& hp();

	/// <summary>
	/// 重力値を取得
	/// </summary>
	float& gravity();

	/// <summary>
	/// 無敵時間を取得
	/// </summary>
	float& invincible_timer();

protected:
	/// <summary>
	/// 重力の更新
	/// </summary>
	void update_gravity(float delta_time);

	/// <summary>
	/// 無敵時間の更新
	/// </summary>
	void update_invincible(float delta_time);

	/// <summary>
	/// メッシュの更新
	/// </summary>
	void update_mesh(float delta_time);

	/// <summary>
	/// 地形との衝突判定
	/// </summary>
	virtual void collide_field();

	/// <summary>
	/// アクターとの衝突判定
	/// </summary>
	virtual void collide_actor(Actor& other);

protected:
	// アニメーションメッシュ
	AnimatedMesh mesh_{ 0, 0, 0, 0 };
	// モーション番号
	GSuint motion_{ 0 };
	// モーションのループ指定
	bool motion_loop_{ true };

	// HP
	int hp_{ 1 };
	// 重力値
	float gravity_{ 0 };
	// 無敵時間タイマー
	float invincible_timer_{ 0 };

};

#endif
