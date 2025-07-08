// -----------------------------------------------------------------------------------------
//  File        : NullState.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/25
//  Description : ステートマシーンエラー回避用空ステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NULL_STATE_H_
#define NULL_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class NullState : public IState {
public:
	virtual void enter() override {}

	virtual void update(float delta_time) override {}

	virtual void draw() const override {}

	virtual void draw_gui() const override {}

	virtual void exit() override {}

};

#endif
