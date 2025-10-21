// -----------------------------------------------------------------------------------------
//  File          : SceneOriginalState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : シーン独自の状態
//                  スタンダードシーン(ステートマシーン)を持つシーンでのみ使用可能

//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_ORIGINAL_STATE_H_
#define SCENE_ORIGINAL_STATE_H_

#include "State/Scene/SceneState.h"

class StandardScene;

class SceneOriginalState : public SceneState {
public:
    SceneOriginalState(StandardScene& owner);

private:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override;

    void exit() override {};

private:
    StandardScene& owner_;

};

#endif
