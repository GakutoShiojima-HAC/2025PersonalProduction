#include "GUI/Button/TextFunctionButton.h"
#include <gslib.h>
#include "GameConfig.h"

static const GScolor DEFAULT_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
static const GScolor HIGHLIGHT_COLOR{ 1.0f, 0.85f, 0.0f, 1.0f };

TextFunctionButton::TextFunctionButton(const std::string& text, const GSvector2& position, GSuint font_size, const Anchor anchor, const Anchor text_anchor) {
	position_ = position;
	font_size_ = font_size;
	anchor_ = anchor;
	text_anchor_ = text_anchor;
	change_text(text);
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

	// カスタムフォントのスタイルを指定
	gsSetSpriteFontStyle(GS_FONT_NORMAL, font_size_, cFONT.c_str());
	// 文字サイズを取得
	GSvector2 text_size{ 0.0f, 0.0f };
	gsGetSpriteFontSize(text.c_str(), &text_size);
	// 基点と差分からスクリーン座標を求める
	const GSvector2 final_position =
		Canvas::get_anchor_position(anchor_) + position_ -
		Canvas::get_anchor_position(text_anchor_, GSrect{ 0.0f, 0.0f, text_size.x, text_size.y }
	);
	// 矩形を再定義
	rect_ = GSrect{ final_position.x, final_position.y, final_position.x + text_size.x, final_position.y + text_size.y };
}
