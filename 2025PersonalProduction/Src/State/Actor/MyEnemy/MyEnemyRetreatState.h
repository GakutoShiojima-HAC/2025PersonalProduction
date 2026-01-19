// -----------------------------------------------------------------------------------------
//  File          : MyEnemyRetreatState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/19
//  Updated       : 2026/01/19
//  Description   : 後退り移動ステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYENEMY_RETREAT_STATE_H_
#define MYENEMY_RETREAT_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class MyEnemy;

class MyEnemyRetreatState : public IState {
public:
    MyEnemyRetreatState(MyEnemy& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop, float retreat_distance);

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

    float retreat_distance_{ 5.0f };

};

#endif
