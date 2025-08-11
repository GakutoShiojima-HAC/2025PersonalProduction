// -----------------------------------------------------------------------------------------
//  File          : PlayerState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : プレイヤーの状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Player;

class PlayerState : public IState {
public:
    PlayerState(Player& owner) : owner_{ owner } {};

public:
    enum class State {
        /* common */
        Idle,           // 待機 (操作を受け付けない)
        Fall,           // 空中
        Land,           // 着地

        /* action */
        Move,           // 移動
        Interact,       // インタラクト
        Attack,         // 攻撃
        Skill,          // スキル
        Jump,           // ジャンプ
        Avoid,          // 回避

        /* react */
        Hurt,           // 負傷
        Dead,           // 死亡

        MAX_SIZE	    // 要素数
    };

protected:
    Player& owner_;

};

using PlayerStateType = PlayerState::State;

#endif
