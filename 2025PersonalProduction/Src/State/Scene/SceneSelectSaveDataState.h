// -----------------------------------------------------------------------------------------
//  File          : SceneSelectSaveDataState.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/01
//  Updated       : 2026/02/01
//  Description   : セーブデータ選択ステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_SELECT_SAVEDATA_STATE_H_
#define SCENE_SELECT_SAVEDATA_STATE_H_

#include "State/Scene/SceneState.h"
#include "Engine/Core/Button/ButtonManager.h"

class MenuScene;

class SceneSelectSaveDataState : public SceneState {
public:
    SceneSelectSaveDataState(MenuScene& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override {};

private:
    void draw_empty_save_data() const;

    void draw_select_save_data() const;

    void start_game();

    void return_state();

private:
    MenuScene& owner_;

    ButtonManager button_;
    ButtonManager button_empty_;

    unsigned int select_index_{ 0 };

    float rotate_icon_{ 0.0f };

};

#endif
