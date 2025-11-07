#include "State/Scene/SceneGameEndState.h"
#include "Scene/GameScene.h"
#include "Engine/Core/World/World.h"
#include "GameConfig.h"

SceneGameEndState::SceneGameEndState(GameScene& owner, World* world) :
    owner_{ owner } {
    world_ = world;

}

void SceneGameEndState::enter() {
    end_timer_ = 0.0f;

    // I—¹‰‰o’†‚ÍGUI•`‰æ‚ğs‚í‚È‚¢
    owner_.enable_draw_game_gui() = false;
}

void SceneGameEndState::update(float delta_time) {
    end_timer_ += delta_time / cFPS;

    // ‰¼ 1•b‚©‚¯‚Ä‘JˆÚ
    if (end_timer_ >= 1.0f) {
        owner_.scene_end_request();
        return;
    }
}

void SceneGameEndState::draw() const {
    // ”wŒi‚Æ‚µ‚ÄƒQ[ƒ€‰æ–Ê‚ğ•`‰æ‚·‚é
    owner_.original_draw();
}

void SceneGameEndState::exit() {
    // GUI•`‰æ‚ğ•œŠˆ
    owner_.enable_draw_game_gui() = true;
}
