// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitatorState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : チュートリアル進行の状態定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIAL_FACILITATOR_STATE_H_
#define TUTORIAL_FACILITATOR_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class TutorialFacilitator;

class TutorialFacilitatorState : public IState {
public:
    TutorialFacilitatorState(TutorialFacilitator& owner) : owner_{ owner } {};

public:
    enum class State {
        Idle,               // プレイヤーを待つ

        AttackInput,        // 攻撃入力を待つ
        AvoidInput,         // 回避入力を待つ
        AvoidAttackInput,   // 回避攻撃入力を待つ
        Event,              // イベント
        Attack,             // 攻撃
        Hurt,               // 負傷
        Dead,               // 死亡(終了)

        MAX_SIZE	    // 要素数
    };

protected:
    TutorialFacilitator& owner_;

};

using TutorialFacilitatorStateType = TutorialFacilitatorState::State;

#endif
