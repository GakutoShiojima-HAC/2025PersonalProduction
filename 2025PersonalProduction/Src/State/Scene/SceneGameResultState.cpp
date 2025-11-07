#include "State/Scene/SceneGameResultState.h"
#include "Scene/GameScene.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"

SceneGameResultState::SceneGameResultState(GameScene& owner, World* world) :
    owner_{ owner } {
    world_ = world;

}

void SceneGameResultState::enter() {
    // リザルト中はGUI描画を行わない
    owner_.enable_draw_game_gui() = false;
    button_.start();
}

void SceneGameResultState::update(float delta_time) {
    Input& input = Input::get_instance();

    // 再度ポーズボタンが押されたらリザルトを終わる(仮)
    if (input.action(InputAction::APP_Pause)) {
        end_result();
        owner_.set_next_scene(SceneTag::Game);
        return;
    }

    button_.update(delta_time);
}

void SceneGameResultState::draw() const {
    // 背景としてゲーム画面を描画する
    owner_.original_draw();

    button_.draw();

    Input::get_instance().draw_cursor();
}

void SceneGameResultState::exit() {

}

void SceneGameResultState::end_result() {
    // GUI描画を復活
    owner_.enable_draw_game_gui() = true;
    owner_.change_state((GSuint)SceneStateType::GameEnd);
}
