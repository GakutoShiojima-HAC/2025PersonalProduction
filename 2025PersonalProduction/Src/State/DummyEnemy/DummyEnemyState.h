// -----------------------------------------------------------------------------------------
//  File          : DummyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/07
//  Updated       : 2025/09/07
//  Description   : ダミーエネミーの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef DUMMY_ENEMY_STATE_H_
#define DUMMY_ENEMY_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class DummyEnemy;

class DummyEnemyState : public IState {
public:
    DummyEnemyState(DummyEnemy& owner) : owner_{ owner } {};

public:
    enum class State {
        /* common */
        Idle,           // 待機 (操作を受け付けない)

        /* action */
        Move,           // 移動
        Attack,         // 攻撃

        /* react */
        Dead,           // 死亡

        MAX_SIZE	    // 要素数
    };

protected:
    DummyEnemy& owner_;

};

using DummyEnemyStateType = DummyEnemyState::State;

#endif
