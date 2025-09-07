#ifndef DUMMY_ENEMY_DEAD_STATE_H_
#define DUMMY_ENEMY_DEAD_STATE_H_

#include "State/DummyEnemy/DummyEnemyState.h"

class DummyEnemyDeadState : public DummyEnemyState {
public:
	DummyEnemyDeadState(DummyEnemy& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	virtual void draw() const override {}

	virtual void draw_gui() const override {}

	void exit() override;

};

#endif