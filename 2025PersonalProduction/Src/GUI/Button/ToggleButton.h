// -----------------------------------------------------------------------------------------
//  File          : ToggleButton.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/22
//  Updated       : 2025/10/22
//  Description   : 文字を描画し、入力があれば関数を実行するボタン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TOGGLE_BUTTON_H_
#define TOGGLE_BUTTON_H_

#include "GUI/Button/TextFunctionButton.h"

typedef	unsigned int GSuint;

class ToggleButton : public TextFunctionButton {
public:
    ToggleButton(bool& toggle, const GSvector2& position,
        GSuint font_size, const Anchor anchor = Anchor::TopLeft, const Anchor text_anchor = Anchor::TopLeft);

public:
    void select() override;

    void state(float delta_time) override;

    void input() override;

    void exit() override;

private:
    bool& toggle_;

};

#endif
