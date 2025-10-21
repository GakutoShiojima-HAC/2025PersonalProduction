#include "State/Scene/SceneGamePauseState.h"
#include "Scene/GameScene.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GUI/Button/TextureFunctionButton.h"

SceneGamePauseState::SceneGamePauseState(GameScene& owner) :
    owner_{ owner } {
    // ボタンの追加
    const GSrect rect{ 0.0f, 0.0f, 450.0f, 110.0f };
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ReturnGameButtonTexture, GSvector2{ 191.0f, 239.0f }, rect };
        button->on_input([=] { return_game(); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ToSettingButtonTexture, GSvector2{ 191.0f, 403.0f }, rect };
        button->on_input([=] { to_setting(); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ToGuideButtonTexture , GSvector2{ 191.0f, 567.0f }, rect };
        button->on_input([=] { to_guide(); });
        button_.add(button);
    }
    {
        TextureFunctionButton* button = new TextureFunctionButton{ (GSuint)TextureID::ReturnTitleButtonTexture, GSvector2{ 191.0f, 731.0f }, rect };
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
