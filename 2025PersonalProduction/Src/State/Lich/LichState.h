// -----------------------------------------------------------------------------------------
//  File          : LichState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/21
//  Updated       : 2025/11/21
//  Description   : Lichの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LICH_STATE_H_
#define LICH_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Lich;

class LichState : public IState {
public:
    LichState(Lich& owner) : owner_{ owner } {};

public:
    enum class State {
        Idle,           // 待機 (操作を受け付けない)

        Move,           // 移動

        Attack,         // 通常攻撃
        Spell,          // 魔法攻撃

        Hurt,           // 負傷
        Dead,           // 死亡

        MAX_SIZE	    // 要素数
    };

protected:
    Lich& owner_;

};

using LichStateType = LichState::State;

#endif
