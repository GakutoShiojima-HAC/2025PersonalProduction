// -----------------------------------------------------------------------------------------
//  File          : MyEnemyAttackState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/13
//  Updated       : 2026/02/13
//  Description   : エネミーの攻撃状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MY_ENEMY_ATTACK_STATE_H_
#define MY_ENEMY_ATTACK_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class MyEnemy;

class MyEnemyAttackState : public IState {
public:
    MyEnemyAttackState(MyEnemy& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop);

public:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override;

private:
    MyEnemy& owner_;

    unsigned int next_state_{ 0 };
    unsigned int next_motion_{ 0 };
    bool motion_loop_{ false };
};

#endif
