// -----------------------------------------------------------------------------------------
//  File          : SceneGameResultState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : ゲーム中のリザルト状態 終了後はゲームシーン終了演出に入る
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_GAME_RESULT_STATE_H_
#define SCENE_GAME_RESULT_STATE_H_

#include "State/Scene/SceneState.h"
#include "Engine/Core/Button/ButtonManager.h"

class TextFunctionButton;
class GameScene;
class World;

class SceneGameResultState : public SceneState {
public:
    SceneGameResultState(GameScene& owner, World* world);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override;

private:
    void try_agein();

    void end_game();

private:
    GameScene& owner_;

    World* world_{ nullptr };

    ButtonManager button_one_;
    ButtonManager button_two_;

    // 文字を変えるために保持しておく
    TextFunctionButton* try_agein_button_{ nullptr };

    // プレイヤーが死亡したか
    bool player_dead_{ false };
    // ステージをクリアしたか
    bool stage_clear_{ false };
    // プレイヤーの復活は可能か
    bool player_respawn_{ false };

    // 最終スコア
    int final_score_{ 0 };
    // 前のスコア
    int prev_score_{ 0 };

};

#endif
