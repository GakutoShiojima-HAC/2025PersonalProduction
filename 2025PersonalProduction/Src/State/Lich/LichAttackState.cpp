#include "State/Lich/LichAttackState.h"
#include "Actor/Enemy/Lich/Lich.h"

LichAttackState::LichAttackState(Lich& owner) :
	LichState(owner) {

}

void LichAttackState::enter() {

}

void LichAttackState::update(float delta_time) {
    if (!owner_.is_motion_end()) return;

    // “G‚ª‚¢‚È‚¯‚ê‚ÎI—¹
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)LichStateType::Idle);
        return;
    }

    // UŒ‚‚ª‘±‚¯‚ê‚é‚È‚çUŒ‚
    if (owner_.current_motion() == LichMotion::Attack1) {
        const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
        const float to_target_length = to_target.magnitude();

        // ”ÍˆÍ“à‚©H
        if (to_target_length < owner_.my_info().attack_detection_radius) {
            owner_.look_target();
            owner_.change_state((GSuint)LichStateType::Attack, LichMotion::Attack2, false);
            return;
        }
    }

    // –ß‚é
    owner_.change_state_and_motion((GSuint)LichStateType::Idle);
}

void LichAttackState::exit() {

}
