// -----------------------------------------------------------------------------------------
//  File          : SceneMenuState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : メニューシーン
//                  スタンダードシーン(ステートマシーン)を持つシーンでのみ使用可能
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_MENU_STATE_H_
#define SCENE_MENU_STATE_H_

#include "State/Scene/SceneState.h"
#include "Engine/Core/Button/ButtonManager.h"

class StandardScene;

class SceneMenuState : public SceneState {
public:
    SceneMenuState(StandardScene& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override {};

private:
    StandardScene& owner_;

    ButtonManager button_;

};

#endif
