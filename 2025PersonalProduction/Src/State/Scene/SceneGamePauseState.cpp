#include "State/Scene/SceneGamePauseState.h"
#include "Scene/GameScene.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"

SceneGamePauseState::SceneGamePauseState(GameScene& owner) :
    owner_{ owner } {
    // ボタンの追加
    const GSrect rect{ 0.0f, 0.0f, 450.0f, 110.0f };
    {
        TextFunctionButton* button = new TextFunctionButton{ "ゲーム再開", GSvector2{ 416.0f, 294.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { return_game(); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "ゲーム設定", GSvector2{ 416.0f, 458.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { to_setting(); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "操作説明", GSvector2{ 416.0f, 622.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { to_guide(); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "タイトルに戻る", GSvector2{ 416.0f, 786.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { return_title(); });
        button_.add(button);
    }
}

void SceneGamePauseState::enter() {
    // ポーズ中はGUI描画を行わない
    owner_.enable_draw_game_gui() = false;
    button_.start();
}

void SceneGamePauseState::update(float delta_time) {
    Input& input = Input::get_instance();

    // 再度ポーズボタンが押されたらゲームに戻る
    if (input.action(InputAction::APP_Pause)) {
        return_game();
        return;
    }

    button_.update(delta_time);
}

void SceneGamePauseState::draw() const {
    // 背景としてゲーム画面を描画する
    owner_.original_draw();

    Canvas::draw_texture((GSuint)TextureID::PauseWindowTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    button_.draw();
}

void SceneGamePauseState::exit() {

}

void SceneGamePauseState::return_game() {
    // GUI描画を復活
    owner_.enable_draw_game_gui() = true;
    owner_.change_state((GSuint)SceneStateType::Original);
}

void SceneGamePauseState::to_setting() {
    owner_.change_state((GSuint)SceneStateType::Setting);
}

void SceneGamePauseState::to_guide() {
    owner_.change_state((GSuint)SceneStateType::Guide);
}

void SceneGamePauseState::return_title() {
    // TODO
    return_game();
}
