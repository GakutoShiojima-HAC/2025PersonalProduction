// -----------------------------------------------------------------------------------------
//  File          : ElementalState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/20
//  Updated       : 2026/01/20
//  Description   : エレメンタルの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ELEMENTAL_STATE_H_
#define ELEMENTAL_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Elemental;

class ElementalState : public IState {
public:
    ElementalState(Elemental& owner) : owner_{ owner } {};

public:
    enum class State {
        Idle,
        Angry,
        MoveF,
        Hurt,
        Dead,

        Attack1,
        Attack2,

        Spell,


        MAX_SIZE	    // 要素数
    };

protected:
    Elemental& owner_;

};

using ElementalStateType = ElementalState::State;

#endif
