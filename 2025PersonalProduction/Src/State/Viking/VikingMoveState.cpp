#include "State/Viking/VikingMoveState.h"
#include "Actor/Enemy/Viking/Viking.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Utils/MyMath.h"

VikingMoveState::VikingMoveState(Viking& owner) :
	VikingState(owner) {

}

void VikingMoveState::enter() {
    owner_.start_move();
    move_timer_ = 0.0f;
}

void VikingMoveState::update(float delta_time) {
    // ƒ^[ƒQƒbƒg‚ª‚¢‚È‚©‚Á‚½‚çƒT[ƒ`‚É–ß‚é
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)VikingStateType::Idle);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // ˆê’èŠÔ‚ÌˆÚ“®‚©õ“G”ÍˆÍŠO‚É‚¢‚Á‚½‚ç
    if (move_timer_ > 5.0f || to_target_length > owner_.my_info().search_length * 1.5f) {

        // “ÁêUŒ‚
        const bool is_rush = MyRandom::random_float(0.0f, 1.0f) <= 0.5f;
        owner_.look_target();
        if (is_rush) {
            owner_.change_state_and_motion((GSuint)VikingStateType::RushCharge);
        }
        else {
            owner_.change_state_and_motion((GSuint)VikingStateType::JumpStart);
        }
        return;
    }

    // UŒ‚‹——£‚É“ü‚Á‚½‚ç
    if (to_target_length < owner_.my_info().attack_data.find(VikingMotion::Attack)->second.detection_radius) {
        // “G‚Ì•ûŒü‚ğŒü‚¢‚Ä‚¢‚é‚©‚Ç‚¤‚©
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            const bool is_attack = MyRandom::random_float(0.0f, 1.0f) <= 0.5f;
            if (is_attack) {
                owner_.change_state_and_motion((GSuint)VikingStateType::Attack);
            }
            else {
                owner_.change_state_and_motion((GSuint)VikingStateType::Block);
            }
        }
        else {
            owner_.update_look_target(delta_time);
        }
        return;
    }

    // ˆÚ“®ŠÔ‚ğŒvZ
    move_timer_ += delta_time / cFPS;
    owner_.update_move(delta_time);
}

void VikingMoveState::exit() {
    owner_.move_end();
}
