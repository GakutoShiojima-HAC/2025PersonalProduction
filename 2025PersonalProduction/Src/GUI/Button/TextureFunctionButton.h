// -----------------------------------------------------------------------------------------
//  File          : TextureFunctionButton.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/21
//  Updated       : 2025/10/21
//  Description   : テクスチャを描画し、入力があれば関数を実行するボタン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TEXTURE_FUNCTION_BUTTON_H_
#define TEXTURE_FUNCTION_BUTTON_H_

#include "Engine/Core/Button/Button.h"
#include <functional>

typedef	unsigned int GSuint;

class TextureFunctionButton : public Button {
public:
    TextureFunctionButton(GSuint texture, const GSvector2& position, const GSrect& rect);

public:
    void draw() const override;

    void select() override;

    void state(float delta_time) override;

    void input() override;

    void exit() override;

public:
    /// <summary>
    /// 入力時のコールバック処理を設定する
    /// </summary>
    /// <param name="input_func">= 関数</param>
    void on_input(std::function<void()> input_func);

protected:
    // 入力時のコールバック
    std::function<void()> input_func_{};

    GSuint texture_{ 0 };

    GSvector2 position_{ 0.0f, 0.0f };

    GSrect texture_rect_{ 0.0f, 0.0f, 0.0f, 0.0f };

};

#endif
