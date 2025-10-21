#include "State/Scene/SceneOriginalState.h"
#include "Engine/Core/Scene/StandardScene.h"

SceneOriginalState::SceneOriginalState(StandardScene& owner) :
    owner_{ owner } {

}

void SceneOriginalState::update(float delta_time) {
    owner_.original_update(delta_time);
}

void SceneOriginalState::draw() const {
    owner_.original_draw();
}
