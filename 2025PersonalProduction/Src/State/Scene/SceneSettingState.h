// -----------------------------------------------------------------------------------------
//  File          : SceneSettingState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : 設定状態
//                  スタンダードシーン(ステートマシーン)を持つシーンでのみ使用可能
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_SETTING_STATE_H_
#define SCENE_SETTING_STATE_H_

#include "State/Scene/SceneState.h"
#include "Engine/Core/Setting/Setting.h"

class StandardScene;

class SceneSettingState : public SceneState {
public:
    SceneSettingState(StandardScene& owner, SceneStateType return_scene);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override {};

private:
    StandardScene& owner_;

    SceneStateType return_scene_;

    Setting& setting_ = Setting::get_instance();
};

#endif
