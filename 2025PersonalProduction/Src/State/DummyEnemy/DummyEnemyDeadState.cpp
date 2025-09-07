#include "State/DummyEnemy/DummyEnemyDeadState.h"
#include "Actor/Enemy/DummyEnemy.h"

DummyEnemyDeadState::DummyEnemyDeadState(DummyEnemy& owner) :
	DummyEnemyState(owner) {
}

void DummyEnemyDeadState::enter() {

}

void DummyEnemyDeadState::update(float delta_time) {
	if (owner_.is_motion_end()) {
		owner_.die();
		return;
	}
}

void DummyEnemyDeadState::exit() {

}