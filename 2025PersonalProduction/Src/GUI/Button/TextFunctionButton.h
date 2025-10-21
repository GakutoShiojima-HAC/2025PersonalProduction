// -----------------------------------------------------------------------------------------
//  File          : TextFunctionButton.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/21
//  Updated       : 2025/10/21
//  Description   : ������`�悵�A���͂�����Ί֐������s����{�^��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
    /// ���͎��̃R�[���o�b�N������ݒ肷��
    /// </summary>
    /// <param name="input_func">= �֐�</param>
    void on_input(std::function<void()> input_func);

protected:
    /// <summary>
    /// �e�L�X�g��ύX���A�����`���Ē�`����
    /// </summary>
    /// <param name="text">= �`�悷��e�L�X�g</param>
    void change_text(const std::string& text);

protected:
    // ���͎��̃R�[���o�b�N
    std::function<void()> input_func_{};

    std::string text_{ "" };

    GSvector2 position_{ 0.0f, 0.0f };

    GSuint font_size_{ 20 };

    // ��ʂ̂ǂ�����_�ɂ��邩
    Anchor anchor_{ Anchor::TopLeft };
    // �e�L�X�g�̂ǂ�����_�ɂ��邩
    Anchor text_anchor_{ Anchor::TopLeft };

};

#endif
