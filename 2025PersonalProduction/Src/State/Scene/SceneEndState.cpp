#include "State/Scene/SceneEndState.h"
#include "Engine/Core/Scene/StandardScene.h"
#include "GameConfig.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"

SceneEndState::SceneEndState(StandardScene& owner) :
    owner_{ owner } {

}

void SceneEndState::enter() {
    end_timer_ = 0.0f;

    GamePostEffect::get_instance().dissolve_threshold() = 1.0f;
}

void SceneEndState::update(float delta_time) {
    // 1•b‚©‚¯‚Ä‘JˆÚ
    if (end_timer_ >= 1.0f) {
        owner_.scene_end_request();
        return;
    }

    GamePostEffect::get_instance().dissolve_threshold() = 1.0f - end_timer_ * 2.0f;
    end_timer_ += delta_time / cFPS;
}

void SceneEndState::draw() const {
    // ”wŒi‚Æ‚µ‚ÄƒQ[ƒ€‰æ–Ê‚ğ•`‰æ‚·‚é
    owner_.original_draw();
}

void SceneEndState::exit() {
    GamePostEffect::get_instance().dissolve_threshold() = 0.0f;
}
