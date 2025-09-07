#include "State/DummyEnemy/DummyEnemyAttackState.h"
#include "Actor/Enemy/DummyEnemy.h"

DummyEnemyAttackState::DummyEnemyAttackState(DummyEnemy& owner) :
	DummyEnemyState(owner) {
}

void DummyEnemyAttackState::enter() {

}

void DummyEnemyAttackState::update(float delta_time) {
	if (owner_.is_motion_end()) {
		owner_.change_state((GSuint)DummyEnemyStateType::Idle, DummyEnemyMotion::Idle, true);
		return;
	}
}

void DummyEnemyAttackState::exit() {

}