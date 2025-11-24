// -----------------------------------------------------------------------------------------
//  File          : LichDeadState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/21
//  Updated       : 2025/11/21
//  Description   : 死亡状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LICH_DEAD_STATE_H_
#define LICH_DEAD_STATE_H_

#include "State/Lich/LichState.h"

class LichDeadState : public LichState {
public:
	 LichDeadState(Lich& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

};

#endif
