// -----------------------------------------------------------------------------------------
//  File          : CharacterIdleState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/03
//  Updated       : 2026/02/03
//  Description   : 汎用アイドルステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_IDLE_STATE_H_
#define CHARACTER_IDLE_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class CharacterIdleState : public IState {
public:
    CharacterIdleState();

private:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

};

#endif
