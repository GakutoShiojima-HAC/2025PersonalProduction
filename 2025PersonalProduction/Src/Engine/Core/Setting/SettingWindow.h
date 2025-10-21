// -----------------------------------------------------------------------------------------
//  File          : SettingWindow.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : ゲーム内設定を描画するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SETTING_WINDOW_H_
#define SETTING_WINDOW_H_

#include "Engine/Core/Button/ButtonManager.h"

class Setting;

class SettingWindow {
public:
    SettingWindow(Setting& owner);

    ~SettingWindow() = default;

public:
    void start();

    void update(float delta_time);

    void draw() const;

    bool is_end() const;

private:
    /// <summary>
    /// 振動設定を有効にしたときのフィードバック処理
    /// </summary>
    void vibration_feedback();

private:
    Setting& setting_;

    bool is_end_{ false };

    ButtonManager button_;

private:
    // 歯車アイコンの回転
    float rotate_gear_{ 0.0f };

public:
    // コピー禁止
    SettingWindow(const SettingWindow& other) = delete;
    SettingWindow& operator = (const SettingWindow& other) = delete;

};

#endif
