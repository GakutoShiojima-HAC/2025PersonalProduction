// -----------------------------------------------------------------------------------------
//  File          : Canvas.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �X�N���[���ɉ�����`�悷��L�����o�X�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------


#ifndef CANVAS_H_
#define CANVAS_H_

#include <string>
#include <gslib.h>

using namespace std;

struct ScreenData;

// ��_
enum class Anchor {
	TopLeft,		TopCenter,		TopRight,
	CenterLeft,		Center,			CenterRight,
	BottomLeft,		BottomCenter,	BottomRight
};

class Canvas {
public:
	// �C���X�^���X�����֎~
	Canvas() = delete;

public:
	/// <summary>
	/// ������
	/// </summary>
	static void init();

public:
	/// <summary>
	/// �e�N�X�`�����X�N���[���ɕ`�悷��
	/// </summary>
	/// <param name="id">= �e�N�X�`��ID</param>
	/// <param name="position">= �X�N���[���`��ʒu</param>
	/// <param name="rect">= �e�N�X�`���̋�`</param>
	/// <param name="center">= �X�v���C�g�̒��S�ʒu</param>
	/// <param name="color">= �J���[</param>
	/// <param name="scale">= �X�P�[���l</param>
	/// <param name="rotation">= ��]�p�x</param>
	/// <param name="anchor">= ��_</param>
	static void draw_texture(
		const GSuint id,
		const GSvector2& position,
		const GSrect& rect,
		const GSvector2& center = GSvector2{ 0.0f, 0.0f },
		const GSvector2& scale = GSvector2{ 1.0f, 1.0f },
		const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
		const GSfloat rotation = 0.0f,
		const Anchor anchor = Anchor::TopLeft
	);

	/// <summary>
	/// �e�L�X�g���X�N���[���ɕ`�悷��
	/// </summary>
	/// <param name="text">= �e�L�X�g</param>
	/// <param name="position">= �X�N���[���`��ʒu</param>
	/// <param name="font_size">= �t�H���g�T�C�Y</param>
	/// <param name="anchor">= ��_</param>
	static void draw_text(
		const string& text,
		const GSvector2& position,
		const GSuint font_size = 20,
		const Anchor anchor = Anchor::TopLeft
	);

	/// <summary>
	/// �X�v���C�g�e�L�X�g���X�N���[���ɕ`�悷��
	/// </summary>
	/// <param name="text">= �e�L�X�g</param>
	/// <param name="position">= �X�N���[���`��ʒu</param>
	/// <param name="font_size">= �t�H���g�T�C�Y</param>
	/// <param name="font_name">= �t�H���g��</param>
	/// <param name="color">= �e�L�X�g�J���[</param>
	/// <param name="anchor">= ��_</param>
	/// <param name="text_anchor">= �e�L�X�g�̊�_</param>
	static void draw_sprite_text(
		const string& text,
		const GSvector2& position,
		const GSuint font_size = 20,
		const string& font_name = "�l�r �S�V�b�N",
		const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
		const Anchor anchor = Anchor::TopLeft,
		const Anchor text_anchor = Anchor::TopLeft
	);

private:
	/// <summary>
	/// ��`�����_�̍��W���擾
	/// </summary>
	/// <param name="anchor">= ��_</param>
	/// <param name="rect">= ��`</param>
	/// <returns>���W</returns>
	static GSvector2 get_anchor_position(const Anchor anchor, const GSrect& rect);

	/// <summary>
	/// ��_�̃X�N���[�����W���擾
	/// </summary>
	/// <param name="anchor">= ��_</param>
	/// <returns>���W</returns>
	static GSvector2 get_anchor_position(const Anchor anchor);

private:
	static ScreenData* screen_data_;

public:
	// �R�s�[�֎~
	Canvas(const Canvas& other) = delete;
	Canvas& operator = (const Canvas& other) = delete;

};

#endif
