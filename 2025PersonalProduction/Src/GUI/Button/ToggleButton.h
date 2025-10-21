// -----------------------------------------------------------------------------------------
//  File          : ToggleButton.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/22
//  Updated       : 2025/10/22
//  Description   : ������`�悵�A���͂�����Ί֐������s����{�^��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
