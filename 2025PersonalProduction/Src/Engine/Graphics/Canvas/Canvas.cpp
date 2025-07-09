#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Screen/Screen.h"

// ���̉�
ScreenData* Canvas::screen_data_{ nullptr };

void Canvas::init() {
	screen_data_ = &Screen::get_instance().get_current_data();;
}

void Canvas::draw_texture(
	const GSuint id, 
	const GSvector2& position, 
	const GSrect& rect, 
	const GSvector2& center, 
	const GSvector2& scale, 
	const GScolor& color, 
	const GSfloat rotation, 
	const Anchor anchor
) {
	// ��_�ƍ�������X�N���[�����W�����߂�
	const GSvector2 final_position = get_anchor_position(anchor) + GSvector2{ position.x, position.y };

	gsDrawSprite2D(id, &final_position, &rect, &center, &color, &scale, rotation);
}

void Canvas::draw_text(
	const string& text, 
	const GSvector2& position, 
	const GSuint font_size,
	const Anchor anchor
) {
	// ��_�ƍ�������X�N���[�����W�����߂�
	const GSvector2 final_position = get_anchor_position(anchor) + position;

	// ���W���w��
	gsTextPos(final_position.x, final_position.y);
	// �t�H���g�̃X�^�C�����w��
	gsFontParameter(0, font_size, "�l�r �S�V�b�N");
	// ������`��
	gsDrawText(text.c_str());
	// ���W�����Z�b�g
	gsTextPos(0.0f, 0.0f);
}

void Canvas::draw_sprite_text(
	const string& text,
	const GSvector2& position,
	const GSuint font_size,
	const string& font_name,
	const GScolor& color,
	const Anchor anchor,
	const Anchor text_anchor
) {
	// �J�X�^���t�H���g�̃X�^�C�����w��
	gsSetSpriteFontStyle(GS_FONT_NORMAL, font_size, font_name.c_str());
	// �����T�C�Y���擾
	GSvector2 text_size{ 0.0f, 0.0f };
	gsGetSpriteFontSize(text.c_str(), &text_size);

	// ��_�ƍ�������X�N���[�����W�����߂�
	const GSvector2 final_position = 
		get_anchor_position(anchor) + position - 
		get_anchor_position(text_anchor, GSrect{0.0f, 0.0f, text_size.x, text_size.y }
	);

	// �J���[�w��
	gsSetSpriteFontColor(&color);
	// ������`��
	gsDrawSpriteFont(&final_position, text.c_str());
}

GSvector2 Canvas::get_anchor_position(const Anchor anchor, const GSrect& rect) {
	const float center_x = (rect.right - rect.left) / 2.0f;
	const float center_y = (rect.bottom - rect.top) / 2.0f;

	switch (anchor) {
	case Anchor::TopLeft:
		return GSvector2{ rect.left, rect.top };
	case Anchor::TopCenter:
		return GSvector2{ center_x, rect.top };
	case Anchor::TopRight:
		return GSvector2{ rect.right, rect.top };
	case Anchor::CenterLeft:
		return GSvector2{ rect.left, center_y };
	case Anchor::Center:
		return GSvector2{ center_x, center_y };
	case Anchor::CenterRight:
		return GSvector2{ rect.right, center_y };
	case Anchor::BottomLeft:
		return GSvector2{ rect.left, rect.bottom };
	case Anchor::BottomCenter:
		return GSvector2{ center_x, rect.bottom };
	case Anchor::BottomRight:
		return GSvector2{ rect.right, rect.bottom };
	default:
		return GSvector2{ rect.left, rect.top };
	}
}

GSvector2 Canvas::get_anchor_position(const Anchor anchor) {
	return get_anchor_position(anchor, GSrect{ 0.0f, 0.0f, (float)screen_data_->width_px, (float)screen_data_->height_px });
}
