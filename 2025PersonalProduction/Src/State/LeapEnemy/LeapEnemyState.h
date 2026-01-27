// -----------------------------------------------------------------------------------------
//  File          : LeapEnemyState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/18
//  Updated       : 2026/01/20
//  Description   : とびかかり攻撃を持つ敵の状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LEAP_ENEMY_STATE_H_
#define LEAP_ENEMY_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class LeapEnemy;

class LeapEnemyState : public IState {
public:
    LeapEnemyState(LeapEnemy& owner) : owner_{ owner } {};

public:
    enum class State {
        Idle,

        Move,

        Hurt,
        Dead,

        Attack,

        LeapStart,
        Leaping,
        LeapEnd,

        MAX_SIZE	    // 要素数
    };

protected:
    LeapEnemy& owner_;

};

using LeapEnemyStateType = LeapEnemyState::State;

#endif
