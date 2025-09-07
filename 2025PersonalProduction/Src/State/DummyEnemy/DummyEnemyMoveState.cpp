#include "State/DummyEnemy/DummyEnemyMoveState.h"
#include "Actor/Enemy/DummyEnemy.h"

DummyEnemyMoveState::DummyEnemyMoveState(DummyEnemy& owner) :
	DummyEnemyState(owner) {
}

void DummyEnemyMoveState::enter() {

}

void DummyEnemyMoveState::update(float delta_time) {
	owner_.update_move(delta_time);

	if (owner_.is_move_end()) {
		owner_.change_state((GSuint)DummyEnemyStateType::Idle, DummyEnemyMotion::Idle, true);
		return;
	}
}

void DummyEnemyMoveState::exit() {

}
