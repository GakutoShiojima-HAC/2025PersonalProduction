#include "State/DummyEnemy/DummyEnemyIdleState.h"
#include "Actor/Enemy/DummyEnemy.h"

DummyEnemyIdleState::DummyEnemyIdleState(DummyEnemy& owner) :
	DummyEnemyState(owner) {
}

void DummyEnemyIdleState::enter() {

}

void DummyEnemyIdleState::update(float delta_time) {
	if (!owner_.is_generate_collider()) return;

	owner_.change_state((GSuint)DummyEnemyStateType::Attack, DummyEnemyMotion::Attack, false);
	return;
}

void DummyEnemyIdleState::exit() {

}
