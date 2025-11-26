// -----------------------------------------------------------------------------------------
//  File          : SceneEndState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : 終了演出状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_END_STATE_H_
#define SCENE_END_STATE_H_

#include "State/Scene/SceneState.h"

class StandardScene;

class SceneEndState : public SceneState {
public:
    SceneEndState(StandardScene& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override;

private:
    StandardScene& owner_;


    float end_timer_{ 0.0f };

};

#endif
