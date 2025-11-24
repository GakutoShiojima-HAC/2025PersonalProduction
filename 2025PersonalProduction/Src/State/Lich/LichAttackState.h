// -----------------------------------------------------------------------------------------
//  File          : LichAttackState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/24
//  Updated       : 2025/11/24
//  Description   : Lichの攻撃状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LICH_ATTACK_STATE_H_
#define LICH_ATTACK_STATE_H_

#include "State/Lich/LichState.h"

class LichAttackState : public LichState {
public:
	 LichAttackState(Lich& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

};

#endif
