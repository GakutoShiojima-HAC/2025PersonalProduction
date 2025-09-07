#ifndef DUMMY_ENEMY_MOVE_STATE_H_
#define DUMMY_ENEMY_MOVE_STATE_H_

#include "State/DummyEnemy/DummyEnemyState.h"

class DummyEnemyMoveState : public DummyEnemyState {
public:
	DummyEnemyMoveState(DummyEnemy& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	virtual void draw() const override {}

	virtual void draw_gui() const override {}

	void exit() override;

};

#endif