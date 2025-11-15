// -----------------------------------------------------------------------------------------
//  File        : EnemyCounter.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/11/14
//  Updated     : 2025/11/14
//  Description : 通常エネミーをカウントするクラス
//                全てのアクターを生成した後に生成すること  
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ENEMY_COUNTER_H_
#define ENEMY_COUNTER_H_

#include "Engine/Core/Actor/Actor.h"

class EnemyCounter : public Actor {
public:
    EnemyCounter(IWorld* world, int enemy_count);

    ~EnemyCounter() = default;

public:
    void update(float delta_time) override;

    void draw_gui() const override;

    void die() override;

private:
    enum class State {
        Idle,
        Event
    };

private:
    // 通常エネミー以外のエネミーの数
    int counter_{ 0 };

    State state_{ State::Idle };

    GSuint player_prev_state_{ 0 };
};

#endif
