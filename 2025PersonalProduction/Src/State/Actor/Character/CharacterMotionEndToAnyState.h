// -----------------------------------------------------------------------------------------
//  File          : CharacterMotionEndToAnyState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/09
//  Updated       : 2026/01/09
//  Description   : モーション再生後指定した任意のステートに戻る
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_MOTION_END_TO_ANYSTATE_STATE_H_
#define CHARACTER_MOTION_END_TO_ANYSTATE_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Character;

class CharacterMotionEndToAnyState : public IState {
public:
    CharacterMotionEndToAnyState(Character& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop);

private:
	void enter() override {};

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override {};

private:
    Character& owner_;

    unsigned int next_state_{ 0 };
    unsigned int next_motion_{ 0 };
    bool motion_loop_{ false };

};

#endif
