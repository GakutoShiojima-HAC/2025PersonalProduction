#include "CharacterMotionEndToDieState.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"

CharacterMotionEndToDieState::CharacterMotionEndToDieState(Pawn& owner) :
    owner_{ owner } {
}

void CharacterMotionEndToDieState::update(float delta_time) {
    if (owner_.is_motion_end()) owner_.die();
}
