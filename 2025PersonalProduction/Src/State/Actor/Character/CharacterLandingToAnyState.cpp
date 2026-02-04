#include "CharacterLandingToAnyState.h"
#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "CharacterIdleState.h"

CharacterLandingToAnyState::CharacterLandingToAnyState(Character& owner, unsigned int next_state, unsigned int next_motion, bool motion_loop) :
    owner_{ owner } {
    next_state_ = next_state;
    next_motion_ = next_motion;
    motion_loop_ = motion_loop;
}

void CharacterLandingToAnyState::update(float delta_time) {
    if (owner_.is_grounded()) {
        owner_.change_state(next_state_, next_motion_, motion_loop_);
        return;
    }
}
