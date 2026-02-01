// -----------------------------------------------------------------------------------------
//  File          : ElementalMoveState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/20
//  Updated       : 2026/01/20
//  Description   : エレメンタルの移動状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ELEMENTAL_MOVE_STATE_H_
#define ELEMENTAL_MOVE_STATE_H_

#include "State/Elemental/ElementalState.h"

class ElementalMoveState : public ElementalState {
public:
    ElementalMoveState(Elemental& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override;

private:
    float attack_time_{ 0.0f };

};

#endif
