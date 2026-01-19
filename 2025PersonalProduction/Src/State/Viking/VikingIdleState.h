// -----------------------------------------------------------------------------------------
//  File          : VikingIdleState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/09
//  Updated       : 2026/01/09
//  Description   : 待機ステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef VIKING_IDLE_STATE_H_
#define VIKING_IDLE_STATE_H_

#include "State/Viking/VikingState.h"

class VikingIdleState : public VikingState {
public:
	 VikingIdleState(Viking& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

};

#endif
