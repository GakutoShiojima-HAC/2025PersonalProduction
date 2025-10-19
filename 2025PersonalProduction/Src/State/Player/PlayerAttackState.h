// -----------------------------------------------------------------------------------------
//  File          : PlayerAttackState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/10/20
//  Description   : プレイヤーの攻撃発動状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_ATTACK_STATE_H_
#define PLAYER_ATTACK_STATE_H_

#include "State/Player/PlayerState.h"

class PlayerAttackState : public PlayerState {
public:
	PlayerAttackState(Player& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

private:
	// 次の段への入力があったかどうか
	bool next_attack_input_{ false };
	// 次の段への遷移タイマー
	float enter_next_attack_timer_{ 0.0f };

};

#endif
