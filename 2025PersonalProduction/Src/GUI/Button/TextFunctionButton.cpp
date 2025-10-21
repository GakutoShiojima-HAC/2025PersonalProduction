#include "GUI/Button/TextFunctionButton.h"
#include <gslib.h>
#include "GameConfig.h"

static const GScolor DEFAULT_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
static const GScolor HIGHLIGHT_COLOR{ 1.0f, 0.85f, 0.0f, 1.0f };

TextFunctionButton::TextFunctionButton(const std::string& text, const GSvector2& position, GSuint font_size, const Anchor anchor, const Anchor text_anchor) {
	change_text(text);
	position_ = position;
	font_size_ = font_size;
	anchor_ = anchor;
	text_anchor_ = text_anchor;
}

void TextFunctionButton::draw() const {
	Canvas::draw_sprite_text(
		text_,
		position_,
		font_size_,
		cFONT,
		GS_FONT_NORMAL,
		is_selected() ? HIGHLIGHT_COLOR : DEFAULT_COLOR,
		anchor_,
		text_anchor_
	);
}

void TextFunctionButton::select() {

}

void TextFunctionButton::state(float delta_time) {

}

void TextFunctionButton::input() {
	if (input_func_) input_func_();
}

void TextFunctionButton::exit() {

}

void TextFunctionButton::on_input(std::function<void()> input_func) {
	input_func_ = input_func;
}

void TextFunctionButton::change_text(const std::string& text) {
	text_ = text;

	// 文字サイズを取得
	GSvector2 text_size{ 0.0f, 0.0f };
	gsGetSpriteFontSize(text.c_str(), &text_size);
	// 矩形を再定義
	rect_ = GSrect{ position_.x, position_.y, position_.x + text_size.x, position_.y + text_size.y };
}
