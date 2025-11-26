#include "SceneStartState.h"
#include "Engine/Core/Scene/StandardScene.h"
#include "GameConfig.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"

SceneStartState::SceneStartState(StandardScene& owner) :
    owner_{ owner } {

}

void SceneStartState::enter() {
    end_timer_ = 0.0f;

    GamePostEffect::get_instance().dissolve_threshold() = 0.0f;
}

void SceneStartState::update(float delta_time) {
    owner_.original_update(delta_time);

    // 1•b‚©‚¯‚Ä‘JˆÚ
    if (end_timer_ >= 1.0f) {
        owner_.change_state((GSuint)SceneStateType::Original);
        return;
    }

    GamePostEffect::get_instance().dissolve_threshold() = end_timer_ * 2.0f;
    end_timer_ += delta_time / cFPS;
}

void SceneStartState::draw() const {
    // ”wŒi‚Æ‚µ‚ÄƒQ[ƒ€‰æ–Ê‚ğ•`‰æ‚·‚é
    owner_.original_draw();
}

void SceneStartState::exit() {
    GamePostEffect::get_instance().dissolve_threshold() = 1.0f;
}
