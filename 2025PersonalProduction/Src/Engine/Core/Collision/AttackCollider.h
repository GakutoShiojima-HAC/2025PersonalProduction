// -----------------------------------------------------------------------------------------
//  File          : AttackCollider.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/08/12
//  Description   : 攻撃判定
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ATTACK_COLLIDER_H_
#define ATTACK_COLLIDER_H_

#include "Engine/Core/Actor/Actor.h"

class AttackCollider : public Actor {
public:
	AttackCollider(
        float radius = 0.0f,
        const GSvector3& center = GSvector3{ 0.0f, 0.0f, 0.0f },
        Actor* owner = nullptr,
        int damage = 0,
        const std::string& name = "AttackCollider",
        float lifespan = 1.0f,
        float delay = 0.0f,
        const GSvector3& force = GSvector3{ 0.0f, 0.0f, 0.0f }
    );

public:
	void update(float delta_time) override;

	void draw() const override;

	void react(Actor& other) override;

public:
	/// <summary>
	/// 再度使えるようにする
	/// </summary>
	/// <param name="radius">= 判定の半径</param>
	/// <param name="center">= 生成判定球の中心</param>
	/// <param name="owner">= 生成主</param>
	/// <param name="damage">= ダメージ値</param>
    /// <param name="name">= 識別名</param>
	/// <param name="lifespan">= 判定の寿命</param>
	/// <param name="delay">= 判定が有効になるまでの遅延</param>
    /// <param name="force">= 吹き飛ばす力</param>
	/// <param name="disable_clear">= マネージャーからのメモリ解放対象から除外するかどうか</param>
	void reactivate(
        float radius = 0.0f,
        const GSvector3& center = GSvector3{ 0.0f, 0.0f, 0.0f },
        Actor* owner = nullptr,
        int damage = 0,
        const std::string& name = "AttackCollider",
        float lifespan = 1.0f,
        float delay = 0.0f,
        const GSvector3& force = GSvector3{ 0.0f, 0.0f, 0.0f },
        bool disable_clear = false
    );

private:
	Actor* owner_{ nullptr };

	// 寿命
	float lifespan_timer_{ 1.0f };
	// 生成遅延
	float delay_timer_{ 0.0f };

	// ダメージ
	int damage_{ 0 };
    // 吹き飛ばす力
    GSvector3 force_{ 0.0f, 0.0f, 0.0f };

};

#endif
