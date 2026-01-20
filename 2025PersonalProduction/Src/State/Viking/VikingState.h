// -----------------------------------------------------------------------------------------
//  File          : VikingState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/18
//  Updated       : 2026/01/20
//  Description   : バイキングの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef VIKING_STATE_H_
#define VIKING_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Viking;

class VikingState : public IState {
public:
    VikingState(Viking& owner) : owner_{ owner } {};

public:
    enum class State {
        Idle,
        Angry,
        Move,
        Retreat,
        Hurt,
        Dead,

        Block,
        BlockHit,

        Attack,

        JumpStart,
        Jumping,
        JumpEnd,

        RushCharge,
        RushStart,
        RushMove,
        RushEnd,


        MAX_SIZE	    // 要素数
    };

protected:
    Viking& owner_;

};

using VikingStateType = VikingState::State;

#endif
