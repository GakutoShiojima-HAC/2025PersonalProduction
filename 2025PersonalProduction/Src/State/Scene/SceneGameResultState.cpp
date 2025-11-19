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

    player_dead_ = world_->count_actor_with_tag(ActorTag::Player) <= 0;
    stage_clear_ = world_->count_actor_with_tag(ActorTag::Enemy) <= 0;
    player_respawn_ = world_->player_respawner().can_respawn();
}

void SceneGameResultState::update(float delta_time) {
    Input& input = Input::get_instance();

    // 再度ポーズボタンが押されたら復活を試みてリザルトを終わる(仮)
    if (input.action(InputAction::APP_Pause)) {
        end_result();
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

    // プレイヤーが死亡したかどうか
    if (player_dead_) {
        // リスポーンができる状態
        if (player_respawn_) {
            owner_.respawn_player();
            owner_.change_state((GSuint)SceneStateType::Original);
            return;
        }
        // リスポーンができない状態
        else {
            // ロビーに戻る
            owner_.set_next_stage(0);
            owner_.set_next_scene(SceneTag::Game);
            owner_.change_state((GSuint)SceneStateType::GameEnd);
            return;
        }
    }

    // ステージをクリアしたかどうか
    if (stage_clear_) {
        // ロビーに戻る
        owner_.set_next_stage(0);
        owner_.set_next_scene(SceneTag::Game);
        owner_.change_state((GSuint)SceneStateType::GameEnd);
        return;
    }
    else {
        // メニューに戻る
        owner_.set_next_scene(SceneTag::Menu);
        owner_.change_state((GSuint)SceneStateType::GameEnd);
        return;
    }
}
