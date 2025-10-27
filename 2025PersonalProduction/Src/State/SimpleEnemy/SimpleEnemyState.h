// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemyState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/24
//  Updated       : 2025/10/24
//  Description   : シンプルエネミーの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_STATE_H_
#define SIMPLE_ENEMY_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class SimpleEnemy;

class SimpleEnemyState : public IState {
public:
    SimpleEnemyState(SimpleEnemy& owner) : owner_{ owner } {};

public:
    enum class State {
        /* common */
        Idle,           // 待機 (操作を受け付けない)

        /* action */
        Search,         // ターゲットを探す
        Find,           // ターゲットへ向く
        Move,           // 移動
        Attack,         // 攻撃

        /* react */
        Hurt,           // 負傷
        Dead,           // 死亡

        MAX_SIZE	    // 要素数
    };

protected:
    SimpleEnemy& owner_;

};

using SimpleEnemyStateType = SimpleEnemyState::State;

#endif
