// -----------------------------------------------------------------------------------------
//  File          : SceneGameEndState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : ゲーム中の終了演出状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_GAME_END_STATE_H_
#define SCENE_GAME_END_STATE_H_

#include "State/Scene/SceneState.h"

class GameScene;
class World;

class SceneGameEndState : public SceneState {
public:
    SceneGameEndState(GameScene& owner, World* world);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override;

private:
    GameScene& owner_;

    World* world_{ nullptr };   // シェーダー用

    float end_timer_{ 0.0f };

};

#endif
