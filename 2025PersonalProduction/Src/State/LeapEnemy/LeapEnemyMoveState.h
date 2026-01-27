// -----------------------------------------------------------------------------------------
//  File          : LeapEnemyMoveState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/20
//  Updated       : 2026/01/20
//  Description   : とびかかり攻撃を持つ敵の移動状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LEAP_ENEMY_MOVE_STATE_H_
#define LEAP_ENEMY_MOVE_STATE_H_

#include "State/LeapEnemy/LeapEnemyState.h"

class LeapEnemyMoveState : public LeapEnemyState {
public:
    LeapEnemyMoveState(LeapEnemy& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override;

private:
    float move_timer_{ 0.0f };
};

#endif
