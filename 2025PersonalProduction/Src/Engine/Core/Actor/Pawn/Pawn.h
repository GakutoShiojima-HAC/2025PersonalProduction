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
#include "Engine/Core/Actor/Pawn/PawnParameter.h"

class AttackCollider;

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

    /// <summary>
    /// 攻撃ヒットコールバック処理
    /// コライダーの寿命は呼ばれたフレームまで
    /// </summary>
    /// <param name="collider">= コライダー</param>
    virtual void on_hit_attack(AttackCollider& collider);

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	virtual void on_jump();

	/// <summary>
	/// 死亡状態かどうか
	/// </summary>
	/// <returns>死亡状態なら真を返却</returns>
	virtual bool is_dead_state() const;

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

	/// <summary>
	/// モーションが終了したかどうか
	/// </summary>
	/// <returns>モーションが終了していたら真を返却</returns>
	bool is_motion_end() const;

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
	virtual void update_mesh(float delta_time);

	/// <summary>
	/// 現在のモーションの総再生時間を取得
	/// </summary>
	/// <returns></returns>
	float current_motion_end_time() const;

protected:
	/// <summary>
	/// 地形との衝突判定
	/// </summary>
	virtual void collide_field() override;

	/// <summary>
	/// アクターとの衝突判定
	/// </summary>
	virtual void collide_actor(Actor& other) override;

    /// <summary>
    /// 空中
    /// </summary>
    virtual void on_air();

    /// <summary>
    /// 接地
    /// </summary>
    virtual void on_ground();


    /// <summary>
    /// パラメータの初期化
    /// </summary>
    void init_parameter(PawnParameter::Type type);

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
	float gravity_{ 9.8f };
	// ジャンプ力
	float jump_power_{ 2.0f };
	// 無敵時間タイマー(秒)
	float invincible_timer_{ 0.0f };

	// 身長
	float height_{ 2.0f };
	
	// 接地しているか
	bool is_ground_{ false };

};

#endif
