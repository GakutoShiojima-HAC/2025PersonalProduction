#include "TutorialFacilitatorEventState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Actor/Player/Player.h"
#include "State/Player/PlayerState.h"

TutorialFacilitatorEventState::TutorialFacilitatorEventState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorEventState::enter() {
    // ターゲットを拘束
    player_prev_state_ = owner_.target()->current_state_num();
    if (owner_.target()->current_state_num() == (GSuint)PlayerStateType::Move) {
        owner_.target()->change_state((GSuint)PlayerStateType::Idle, (GSuint)PlayerMotion::Idle, true);
    }
    else {
        owner_.target()->change_state((GSuint)PlayerStateType::Idle);
    }
    input_.disable_action(InputAction::GAME_Camera) = true;
    owner_.enable_game_info_gui() = false;

    timer_ = 0.0f;
}

void TutorialFacilitatorEventState::update(float delta_time) {
    // もしモーションが止まったらアイドルループ
    if (owner_.is_motion_end()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Event, TutorialFacilitatorMotion::Idle, true);
    }
    
    // 決定入力でイベント開始
    if (input_.action(InputAction::MENU_Decision_State)) {
        timer_ += delta_time / cFPS;
    }
    else {
        timer_ = 0.0f;
    }

    if (timer_ >= 0.75f) {
        owner_.change_prev_state(); //イベントが格納されている前提
        return;
    }
}

void TutorialFacilitatorEventState::exit() {
    // ターゲットの拘束を解除
    owner_.target()->change_state(player_prev_state_);
    input_.disable_action(InputAction::GAME_Camera) = false;
    owner_.enable_game_info_gui() = true;
}
