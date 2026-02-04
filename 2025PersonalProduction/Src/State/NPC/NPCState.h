// -----------------------------------------------------------------------------------------
//  File          : NPCState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/03
//  Updated       : 2026/02/03
//  Description   : NPCの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NPC_STATE_H_
#define NPC_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class NPC;

class NPCState : public IState {
public:
    NPCState(NPC& owner) : owner_{ owner } {};

public:
    enum class State {
        Idle,           // 待機
        Action,         // 会話
        MAX_SIZE	    // 要素数
    };

protected:
    NPC& owner_;

};

using NPCStateType = NPCState::State;

#endif
