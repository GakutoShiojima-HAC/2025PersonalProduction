// -----------------------------------------------------------------------------------------
//  File          : VikingMoveState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/09
//  Updated       : 2026/01/09
//  Description   : 移動ステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef VIKING_MOVE_STATE_H_
#define VIKING_MOVE_STATE_H_

#include "State/Viking/VikingState.h"

class VikingMoveState : public VikingState {
public:
	 VikingMoveState(Viking& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

private:
    float move_timer_{ 0.0f };

};

#endif
