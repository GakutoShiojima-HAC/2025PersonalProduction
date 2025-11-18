// -----------------------------------------------------------------------------------------
//  File        : CinemaActor.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/11/18
//  Updated     : 2025/11/18
//  Description : 特定の条件で登録したタイムラインを再生する  
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CINEMA_ACTOR_H_
#define CINEMA_ACTOR_H_

#include "Engine/Core/Actor/Actor.h"

class ICinemaBehavior;

class CinemaActor : public Actor {
public:
    CinemaActor(IWorld* world, const std::string& timeline_name, bool is_playing_fixed_player = true);

    ~CinemaActor();

public:
    void update(float delta_time) override;

    void draw() const override;

    void draw_gui() const override;

    void die() override;

    void react(Actor& other) override;

public:
    CinemaActor& set_position(const GSvector3& position);

    CinemaActor& set_rotation(const GSvector3& rotate);

    CinemaActor& set_collider(float radius, float height);

    CinemaActor& set_behavior(ICinemaBehavior* behavior);

private:
    void end();

private:
    ICinemaBehavior* behavior_{ nullptr };

    std::string timeline_name_{ "" };

    bool is_player_fixed_{ false };

    GSuint player_prev_state_{ 0 };

    bool play_start_{ false };

};



#endif
