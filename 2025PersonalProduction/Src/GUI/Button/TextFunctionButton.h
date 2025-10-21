// -----------------------------------------------------------------------------------------
//  File          : TextFunctionButton.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/21
//  Updated       : 2025/10/21
//  Description   : 文字を描画し、入力があれば関数を実行するボタン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TEXT_FUNCTION_BUTTON_H_
#define TEXT_FUNCTION_BUTTON_H_

#include "Engine/Core/Button/Button.h"
#include <functional>
#include <string>
#include "Engine/Graphics/Canvas/Canvas.h"

typedef	unsigned int GSuint;

class TextFunctionButton : public Button {
public:
    TextFunctionButton(const std::string& text, const GSvector2& position,
        GSuint font_size, const Anchor anchor = Anchor::TopLeft, const Anchor text_anchor = Anchor::TopLeft);

public:
    virtual void draw() const override;

    virtual void select() override;

    virtual void state(float delta_time) override;

    virtual void input() override;

    virtual void exit() override;

public:
    /// <summary>
    /// 入力時のコールバック処理を設定する
    /// </summary>
    /// <param name="input_func">= 関数</param>
    void on_input(std::function<void()> input_func);

protected:
    /// <summary>
    /// テキストを変更し、判定矩形を再定義する
    /// </summary>
    /// <param name="text">= 描画するテキスト</param>
    void change_text(const std::string& text);

protected:
    // 入力時のコールバック
    std::function<void()> input_func_{};

    std::string text_{ "" };

    GSvector2 position_{ 0.0f, 0.0f };

    GSuint font_size_{ 20 };

    // 画面のどこを基点にするか
    Anchor anchor_{ Anchor::TopLeft };
    // テキストのどこを基点にするか
    Anchor text_anchor_{ Anchor::TopLeft };

};

#endif
