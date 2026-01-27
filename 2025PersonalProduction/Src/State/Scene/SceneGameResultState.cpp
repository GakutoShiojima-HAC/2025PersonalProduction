#include "State/Scene/SceneGameResultState.h"
#include "Scene/GameScene.h"
#include "Engine/Core/World/World.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"
#include "Engine/Sound/BGMManager.h"

SceneGameResultState::SceneGameResultState(GameScene& owner, World* world) :
    owner_{ owner } {
    world_ = world;
    // ボタンの追加
    {
        TextFunctionButton* button = new TextFunctionButton{ "町へ", GSvector2{ 0.0f, 860.0f }, 64, Anchor::TopCenter, Anchor::Center };
        button->on_input([=] { end_game(); });
        button_one_.add(button);
    }
    {
        try_agein_button_ = new TextFunctionButton{ "復活", GSvector2{ -250.0f, 860.0f }, 64, Anchor::TopCenter, Anchor::Center };
        try_agein_button_->on_input([=] { try_agein(); });
        button_two_.add(try_agein_button_);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "戻る", GSvector2{ 250.0f, 860.0f }, 64, Anchor::TopCenter, Anchor::Center };
        button->on_input([=] { end_game(); });
        button_two_.add(button);
    }
}

void SceneGameResultState::enter() {
    // リザルト中はGUI描画を行わない
    owner_.enable_draw_game_gui() = false;
    // ブラーをかける
    world_->set_blur_effect_power() = 0.75f;

    button_one_.start();
    button_two_.start();

    player_dead_ = world_->count_actor_with_tag(ActorTag::Player) <= 0;
    stage_clear_ = world_->count_actor_with_tag(ActorTag::Enemy) <= 0;
    player_respawn_ = world_->player_respawner().can_respawn();

    // テキストを変える
    if (try_agein_button_ != nullptr) try_agein_button_->change_text(player_respawn_ ? "復活" : "再挑戦");

    // スコアを計算
    if (stage_clear_) {
        final_score_ = world_->game_save_data().calc_final_score(world_->action_score().get_total_score(), world_->time().get_elapsed_time(), world_->player_respawner().respawn_count());
        prev_score_ = world_->game_save_data().get_stage_score(owner_.get_current_load_stage());
    }
    BGMManager& bgm = BGMManager::get_instance();
    bgm_handle_ = bgm.current();
    bgm.play(0, 1.0f);
}

void SceneGameResultState::update(float delta_time) {
    if (player_dead_) button_two_.update(delta_time);
    else button_one_.update(delta_time);
}

void SceneGameResultState::draw() const {
    // 背景としてゲーム画面を描画する
    owner_.original_draw();

    // テキスト描画
    auto draw_text = [](
        const std::string& text, const GSvector2& position, int font_size, Anchor anchor, Anchor text_anchor,
        const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) {
        Canvas::draw_sprite_text(
            text,
            position,
            font_size,
            cFONT,
            GS_FONT_NORMAL,
            color,
            anchor,
            text_anchor
        );
    };

    // テキスト位置
    const float main_text_y{ 250.0f };
    const float sub_text_y{ 380.0f };

    if (player_dead_) {
        button_two_.draw();

        draw_text(
            "死んでしまった！",
            GSvector2{ 0.0f, main_text_y }, 150, Anchor::TopCenter, Anchor::Center
        );

        draw_text(
            player_respawn_ ? "ボス戦闘をやり直すことができるようだ..." : "次はきっとうまくいく...",
            GSvector2{ 0.0f, sub_text_y }, 60, Anchor::TopCenter, Anchor::Center
        );
    }
    else {
        button_one_.draw();

        draw_text(
            stage_clear_ ? "ステージクリア！" : "どうしてここに...？",
            GSvector2{ 0.0f, main_text_y }, 150, Anchor::TopCenter, Anchor::Center
        );

        if (stage_clear_) {
            // テキスト位置
            const float text_y{ 55.0f };

            ActionScore& score = world_->action_score();

            draw_text(
                "クリアタイム",
                GSvector2{ -450.0f, sub_text_y }, 40, Anchor::TopCenter, Anchor::CenterLeft
            );
            draw_text(
                world_->time().get_elapsed_time_string(),
                GSvector2{ 450.0f, sub_text_y }, 40, Anchor::TopCenter, Anchor::CenterRight
            );

            draw_text(
                "復活回数",
                GSvector2{ -450.0f, sub_text_y + text_y * 1 }, 40, Anchor::TopCenter, Anchor::CenterLeft
            );
            draw_text(
                std::to_string(world_->player_respawner().respawn_count()),
                GSvector2{ 450.0f, sub_text_y + text_y * 1 }, 40, Anchor::TopCenter, Anchor::CenterRight
            );

            draw_text(
                "総コンボポイント",
                GSvector2{ -450.0f, sub_text_y + text_y * 2 }, 40, Anchor::TopCenter, Anchor::CenterLeft
            );
            draw_text(
                std::to_string(score.get_total_score()),
                GSvector2{ 450.0f, sub_text_y + text_y * 2 }, 40, Anchor::TopCenter, Anchor::CenterRight
            );

            draw_text(
                "最大コンボ数",
                GSvector2{ -450.0f, sub_text_y + text_y * 3 }, 40, Anchor::TopCenter, Anchor::CenterLeft
            );
            draw_text(
                std::to_string(score.get_max_combo_count()),
                GSvector2{ 450.0f, sub_text_y + text_y * 3 }, 40, Anchor::TopCenter, Anchor::CenterRight
            );

            draw_text(
                "最大コンボポイント",
                GSvector2{ -450.0f, sub_text_y + text_y * 4 }, 40, Anchor::TopCenter, Anchor::CenterLeft
            );
            draw_text(
                std::to_string(score.get_max_combo_score()),
                GSvector2{ 450.0f, sub_text_y + text_y * 4 }, 40, Anchor::TopCenter, Anchor::CenterRight
            );

            draw_text(
                "最終スコア",
                GSvector2{ 0.0f, 670.0f }, 60, Anchor::TopCenter, Anchor::Center
            );
            draw_text(
                std::to_string(final_score_),
                GSvector2{ 0.0f, 740.0f }, 70, Anchor::TopCenter, Anchor::Center
            );
            if (final_score_ > prev_score_) {
                draw_text(
                    "新記録！",
                    GSvector2{ 200.0f, 675.0f }, 50, Anchor::TopCenter, Anchor::Center
                );
            }
        }
    }

    Input::get_instance().draw_cursor();
}

void SceneGameResultState::exit() {

}

void SceneGameResultState::try_agein() {
    // GUI描画を復活
    owner_.enable_draw_game_gui() = true;
    // ブラーを解除
    world_->set_blur_effect_power() = 0.0f;
    // リスポーン
    if (player_respawn_) {
        owner_.respawn_player();
        owner_.change_state((GSuint)SceneStateType::Original);
        BGMManager& bgm = BGMManager::get_instance();
        bgm.play(bgm_handle_, 0.1f);
        return;
    }
    // 再挑戦
    else {
        owner_.set_next_scene(SceneTag::Game);
        owner_.change_state((GSuint)SceneStateType::End);
        return;
    }
}

void SceneGameResultState::end_game() {
    // GUI描画を復活
    owner_.enable_draw_game_gui() = true;
    // ブラーを解除
    world_->set_blur_effect_power() = 0.0f;
    if (stage_clear_) {
        // クリアしたのでセーブする
        world_->game_save_data().set_clear_stage(owner_.get_current_load_stage(), final_score_);
    }
    // ロビーに戻る
    owner_.set_next_stage(0);
    owner_.set_next_scene(SceneTag::Game);
    owner_.change_state((GSuint)SceneStateType::End);
}
