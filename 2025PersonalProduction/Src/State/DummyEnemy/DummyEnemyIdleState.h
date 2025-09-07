// -----------------------------------------------------------------------------------------
//  File          : DummyEnemyIdleState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/07
//  Updated       : 2025/09/07
//  Description   : ダミーエネミーの待機状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef DUMMY_ENEMY_IDLE_STATE_H_
#define DUMMY_ENEMY_IDLE_STATE_H_

#include "State/DummyEnemy/DummyEnemyState.h"

class DummyEnemyIdleState : public DummyEnemyState {
public:
	DummyEnemyIdleState(DummyEnemy& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	virtual void draw() const override {}

	virtual void draw_gui() const override {}

	void exit() override;

};

#endif