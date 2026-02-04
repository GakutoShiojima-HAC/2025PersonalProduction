#include "NPCActionState.h"
#include "Actor/NPC/NPC.h"

NPCActionState::NPCActionState(NPC& owner) :
    NPCState{ owner } {

}

void NPCActionState::update(float delta_time) {
    if (owner_.is_action_end()) {
        owner_.end_action();
        owner_.change_state((GSuint)NPCStateType::Idle, owner_.data().motion_idle, true);
        return;
    }
}
