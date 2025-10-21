// -----------------------------------------------------------------------------------------
//  File          : ButtonManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : ボタンを管理するクラス
//                  ゲームパッドの時、ボタンは追加順で選択される
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef BUTTON_MANAGER_H_
#define BUTTON_MANAGER_H_

#include <vector>
#include "Engine/Core/Input/Input.h"

class Button;

class ButtonManager {
public:
    ButtonManager() = default;

    ~ButtonManager();

public:
    void start();

    void update(float delta_time);

    void draw() const;

    void clear();

    Button* add(Button* button);

private:
    /// <summary>
    /// ゲームパッドの選択の更新
    /// </summary>
    void update_select_index();

private:
    Input& input_ = Input::get_instance();

    // 管理しているボタン
    vector<Button*> buttons_;
    // ゲームパッドの選択順番号
    unsigned int select_index_{ 0 };

public:
    // コピー禁止
    ButtonManager(const ButtonManager& other) = delete;
    ButtonManager& operator = (const ButtonManager& other) = delete;

};

#endif
