// -----------------------------------------------------------------------------------------
//  File          : CharacterMotionEndToDieState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/09
//  Updated       : 2026/01/09
//  Description   : モーション再生後指定した死亡する
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_MOTION_END_TO_DIE_STATE_H_
#define CHARACTER_MOTION_END_TO_DIE_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Pawn;

class CharacterMotionEndToDieState : public IState {
public:
    CharacterMotionEndToDieState(Pawn& owner);

private:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

private:
    Pawn& owner_;

};

#endif
