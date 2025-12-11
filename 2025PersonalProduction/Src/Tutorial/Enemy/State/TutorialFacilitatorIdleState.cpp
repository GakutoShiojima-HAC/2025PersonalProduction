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
        // ターゲットを見る
        owner_.look_target();
        // HPを初期化
        owner_.hp() = owner_.my_info().hp;
        // イベント後のステートに一度変更
        owner_.change_state((GSuint)TutorialFacilitatorStateType::AttackInput, TutorialFacilitatorMotion::Idle, true);
        // ステートを記憶
        owner_.save_current_state();
        // イベントに遷移
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Event, TutorialFacilitatorMotion::Idle, true);
        // イベントを再生
        owner_.on_attack_input_event();
        return;
    }
}

void TutorialFacilitatorIdleState::exit() {

}
