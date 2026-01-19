// -----------------------------------------------------------------------------------------
//  File          : MyEnemyLeapAttackState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/19
//  Updated       : 2026/01/19
//  Description   : とびかかり攻撃状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYENEMY_LEAP_ATTACK_STATE_H_
#define MYENEMY_LEAP_ATTACK_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class MyEnemy;

class MyEnemyLeapAttackState : public IState {
public:
    MyEnemyLeapAttackState(MyEnemy& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop, float jump_height);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override;

private:
    void next_state();

private:
    MyEnemy& owner_;

    unsigned int next_state_{ 0 };
    unsigned int next_motion_{ 0 };
    bool motion_loop_{ false };

    // 飛びあがる高さ
    float jump_height_{ 1.0f };
};

#endif
