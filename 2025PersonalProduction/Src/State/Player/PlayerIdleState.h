// -----------------------------------------------------------------------------------------
//  File          : PlayerIdleState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : プレイヤーの待機状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_IDLE_STATE_H_
#define PLAYER_IDLE_STATE_H_

#include "State/Player/PlayerState.h"

class PlayerIdleState : public PlayerState {
public:
	PlayerIdleState(Player& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	void draw() const override;

	void draw_gui() const override;

	void exit() override;

};

#endif
