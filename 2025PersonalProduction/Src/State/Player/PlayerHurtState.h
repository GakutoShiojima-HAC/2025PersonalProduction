// -----------------------------------------------------------------------------------------
//  File          : PlayerHurtState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : プレイヤーの負傷状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_HURT_STATE_H_
#define PLAYER_HURT_STATE_H_

#include "State/Player/PlayerState.h"

class PlayerHurtState : public PlayerState {
public:
	PlayerHurtState(Player& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	void draw() const override;

	void draw_gui() const override;

	void exit() override;

};

#endif
