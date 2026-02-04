#include "TutorialFacilitatorChaseState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"

TutorialFacilitatorChaseState::TutorialFacilitatorChaseState(TutorialFacilitator& owner) :
    TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorChaseState::enter() {
    owner_.start_move();
}

void TutorialFacilitatorChaseState::update(float delta_time) {
    // ターゲットがいなかったら待機状態に戻る
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 近いか
    const bool is_in_radius = to_target_length < owner_.my_info().attack_data.find((GSuint)TutorialFacilitatorMotion::Attack)->second.detection_radius;

    // 近すぎたら見るだけ
    if (is_in_radius) {
        // モーションだけ更新
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Chase, (GSuint)TutorialFacilitatorMotion::Idle, true);
        owner_.update_look_target(delta_time);
    }
    else {
        // 見失ったら待機状態に戻る
        if (to_target_length > owner_.my_info().search_length * 4.0f) {
            owner_.release_target();
            owner_.change_state((GSuint)TutorialFacilitatorStateType::Idle, (GSuint)TutorialFacilitatorMotion::Idle, true);
            return;
        }
        // モーションだけ更新
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Chase, (GSuint)TutorialFacilitatorMotion::Move, true);
        // 追跡
        owner_.update_move(delta_time);
    }
}

void TutorialFacilitatorChaseState::exit() {
    owner_.move_end();
}

