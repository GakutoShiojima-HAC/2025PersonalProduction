#include "State/Scene/SceneSettingState.h"
#include "Engine/Core/Scene/StandardScene.h"

SceneSettingState::SceneSettingState(StandardScene& owner, SceneStateType return_scene) :
    owner_{ owner },
    return_scene_{ return_scene } {

}

void SceneSettingState::enter() {
    setting_.start();
}

void SceneSettingState::update(float delta_time) {
    setting_.update(delta_time);

    if (setting_.is_end()) {
        owner_.change_state((GSuint)return_scene_);
        return;
    }
}

void SceneSettingState::draw() const {
    owner_.original_draw();
    setting_.draw();
}
