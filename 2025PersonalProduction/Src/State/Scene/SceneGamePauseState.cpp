#include "State/Scene/SceneGamePauseState.h"
#include "Scene/GameScene.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"

// ポーズテキストのアニメーション速度
static const float TEXT_ANIME_SPEED{ 0.08f };
// ポーズテキストの最大アニメーション移動量
static const float TEXT_ANIME_MAX_VELOCITY{ 11.0f };

SceneGamePauseState::SceneGamePauseState(GameScene& owner) :
    owner_{ owner } {
    // ボタンの追加
    {
        TextFunctionButton* button = new TextFunctionButton{ "ゲーム再開", GSvector2{ 416.0f, 294.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { return_game(); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "ゲーム設定", GSvector2{ 416.0f, 458.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.change_state((GSuint)SceneStateType::Setting); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "操作説明", GSvector2{ 416.0f, 622.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.change_state((GSuint)SceneStateType::Guide); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "タイトルに戻る", GSvector2{ 416.0f, 786.0f }, 64, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.scene_end_request(); });
        button_.add(button);
    }
}

void SceneGamePauseState::enter() {
    // ポーズ中はGUI描画を行わない
    owner_.enable_draw_game_gui() = false;
    button_.start();
    pause_text_anime_ = 0.0f;
}

void SceneGamePauseState::update(float delta_time) {
    Input& input = Input::get_instance();

    // 再度ポーズボタンが押されたらゲームに戻る
    if (input.action(InputAction::APP_Pause)) {
        return_game();
        return;
    }

    button_.update(delta_time);
    pause_text_anime_ += delta_time * TEXT_ANIME_SPEED;
}

void SceneGamePauseState::draw() const {
    // 背景としてゲーム画面を描画する
    owner_.original_draw();

    Canvas::draw_texture((GSuint)TextureID::PauseWindowTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    button_.draw();

    auto draw_dot_texture = [](const GSvector2& position) {
        Canvas::draw_texture((GSuint)TextureID::DotIconTexture,
            position,
            GSrect{ 0.0f, 0.0f, 11.0f, 11.0f });
    };

    draw_dot_texture(GSvector2{ 1591.0f, 912.0f - CLAMP(std::sin(pause_text_anime_), 0.0f, 1.0f) * TEXT_ANIME_MAX_VELOCITY });
    draw_dot_texture(GSvector2{ 1604.0f, 912.0f - CLAMP(std::sin(pause_text_anime_ - 7.0f), 0.0f, 1.0f) * TEXT_ANIME_MAX_VELOCITY });
    draw_dot_texture(GSvector2{ 1617.0f, 912.0f - CLAMP(std::sin(pause_text_anime_ - 14.0f), 0.0f, 1.0f) * TEXT_ANIME_MAX_VELOCITY });

    Input::get_instance().draw_cursor();
}

void SceneGamePauseState::exit() {

}

void SceneGamePauseState::return_game() {
    // GUI描画を復活
    owner_.enable_draw_game_gui() = true;
    owner_.change_state((GSuint)SceneStateType::Original);
}

