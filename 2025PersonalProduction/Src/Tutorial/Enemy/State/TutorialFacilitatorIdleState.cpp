#include "TutorialFacilitatorIdleState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"

TutorialFacilitatorIdleState::TutorialFacilitatorIdleState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorIdleState::enter() {

}

void TutorialFacilitatorIdleState::update(float delta_time) {
    // プレイヤーが見つかったらイベント開始
    if (owner_.search_target()) {
        owner_.init_tutorial();
        owner_.change_state((GSuint)TutorialFacilitatorStateType::AttackInput, TutorialFacilitatorMotion::Idle, true);
        return;
    }
}

void TutorialFacilitatorIdleState::exit() {

}
