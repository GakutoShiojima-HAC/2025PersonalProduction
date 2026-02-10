// -----------------------------------------------------------------------------------------
//  File          : FireBall.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/25
//  Updated       : 2025/11/25
//  Description   : 魔法
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef FIREBALL_H_
#define FIREBALL_H_

#include "Engine/Core/Actor/Actor.h"

class FireBall : public Actor {
public:
	FireBall(IWorld* world, const GSvector3& position, const GSvector3& velocity, float life_time, Actor* owner, int damage);

	~FireBall() = default;

public:
	void update(float delta_time) override;

    void late_update(float delta_time) override;

public:
	void die() override;

    void react(Actor& other) override;

private:
    /// <summary>
    /// 衝突
    /// </summary>
    void hit();

private:
    Actor* owner_{ nullptr };

    float life_time_{ 0.0f };

    float timer_{ 0.0f };

    int effect_handle_{ 0 };

    int damage_{ 1 };

};

#endif
