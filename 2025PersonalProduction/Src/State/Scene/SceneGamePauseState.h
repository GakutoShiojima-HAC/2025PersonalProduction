// -----------------------------------------------------------------------------------------
//  File          : SceneGamePauseState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : ゲーム中のポーズ状態
//                  ゲームシーンでのみ使用可能
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_GAME_PAUSE_STATE_H_
#define SCENE_GAME_PAUSE_STATE_H_

#include "State/Scene/SceneState.h"
#include "Engine/Core/Button/ButtonManager.h"

class GameScene;

class SceneGamePauseState : public SceneState {
public:
    SceneGamePauseState(GameScene& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override;

private:
    void return_game();

private:
    GameScene& owner_;

    ButtonManager button_;

};

#endif
