// -----------------------------------------------------------------------------------------
//  File          : ExplodeRoad.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/25
//  Updated       : 2025/11/25
//  Description   : 魔法
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef EXPLODEROAD_H_
#define EXPLODEROAD_H_

#include "Engine/Core/Actor/Actor.h"

class ExplodeRoad : public Actor {
public:
	ExplodeRoad(IWorld* world, const GSvector3& position, const GSvector3& velocity, Actor* owner, int damage, int time, float interval);

	~ExplodeRoad() = default;

public:
	void update(float delta_time) override;

public:
	//void die() override;

    //void react(Actor& other) override;

	//void message(const std::string& message, std::any& param) override;

	//bool can_interact() const override;

	//std::string interact_text() const override;

private:
    Actor* owner_{ nullptr };

    int effect_handle_{ 0 };

    int damage_{ 1 };

    int time_{ 1 };

    int counter_{ 0 };

    float interval_{ 0.0f };

    float timer_{ 0.0f };

};

#endif
