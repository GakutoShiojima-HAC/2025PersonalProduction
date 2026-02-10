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
    /// </summary>
    /// <param name="collider">= コライダー</param>
    virtual void on_hit_attack(AttackCollider& collider);

	/// <summary>
	/// 死亡状態かどうか
	/// </summary>
	/// <returns>死亡状態なら真を返却</returns>
	virtual bool is_dead_state() const;

    /// <summary>
    /// 急所のワールド座標を取得
    /// </summary>
    /// <returns>急所のワールド座標</returns>
    virtual GSmatrix4 critical_position() const;

public:
	/// <summary>
	/// HPを取得
	/// </summary>
	int& hp();

    /// <summary>
    /// 最大HPを取得
    /// </summary>
    /// <returns></returns>
    int max_hp() const;

	/// <summary>
	/// 無敵時間を取得
	/// </summary>
	float& invincible_timer();

	/// <summary>
	/// モーションが終了したかどうか
	/// </summary>
	/// <returns>モーションが終了していたら真を返却</returns>
	bool is_motion_end() const;

    /// <summary>
    /// 現在のモーション
    /// </summary>
    /// <returns>モーション番号</returns>
    GSuint current_motion() const;

    /// <summary>
    /// 現在のモーションの再生時間
    /// </summary>
    /// <returns></returns>
    float currnent_motion_time() const;

    /// <summary>
    /// 現在のモーションがループしているか
    /// </summary>
    /// <returns>ループの状態</returns>
    bool current_motion_loop() const;

    /// <summary>
	/// 現在のモーションの総再生時間を取得
	/// </summary>
	/// <returns></returns>
	float current_motion_end_time() const;

    /// <summary>
    /// モーションを停止
    /// </summary>
    /// <returns>参照</returns>
    bool& freeze_motion();

protected:
	/// <summary>
	/// 無敵時間の更新
	/// </summary>
	void update_invincible(float delta_time);

    /// <summary>
    /// 描画用HPの値の更新
    /// </summary>
    void update_display_hp(float delta_time);

    /// <summary>
    /// ポーンパラメータの初期化
    /// </summary>
    void init_parameter(PawnParameter::Type type);

protected:
    /// <summary>
    /// メッシュの更新
    /// </summary>
    virtual void update_mesh(float delta_time);

    /// <summary>
    /// ルートモーションを使う状態かどうか
    /// </summary>
    /// <returns>ルートモーションを使うなら真を返却</returns>
    virtual bool is_root_motion_state() const { return false; };

protected:
	// アニメーションメッシュ
	AnimatedMesh mesh_{ 0, 0, 0, 0 };
	// モーション番号
	GSuint motion_{ 0 };
	// モーションのループ指定
	bool motion_loop_{ true };

	// HP
	int hp_{ 1 };
    // 描画用のHP
    float display_hp_{ 1.0f };
    // 最大HP
    int max_hp_{ 1 };
	// 無敵時間タイマー(秒)
	float invincible_timer_{ 0.0f };

    // モーションを止めるかどうか
    bool is_motion_freeze_{ false };

};

#endif
