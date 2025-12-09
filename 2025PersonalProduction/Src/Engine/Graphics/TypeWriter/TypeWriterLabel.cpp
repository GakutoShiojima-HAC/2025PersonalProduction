#include "TypeWriterLabel.h"
#include <gslib.h>
#include "Assets.h"
#include "GameConfig.h"
#include "Engine/Graphics/Canvas/Canvas.h"

// 背景の座標
const GSvector2 BG_POSITION{ 636.0f, 740.0f };
// テキスト描画の位置
const GSvector2 TEXT_POSITION{ 673.0f, 755.0f };
// 行間
constexpr float ROW_SPACE{ -10.0f };
// フォントサイズ
constexpr float FONT_SIZE{ 46.0f };

void TypeWriterLabel::update(float delta_time) {
    type_writer_.update(delta_time);
}

void TypeWriterLabel::draw() const {
    if (type_writer_.is_empty()) return;

    // 描画するテキストを取得
    const std::vector<std::string>& text = type_writer_.get();
    if (text.empty()) return;

    // 背景を描画
    {
        const GSrect rect{ 0.0f, 0.0f, 648.0, 149.0f };
        Canvas::draw_texture(
            (GSuint)TextureID::TypeWriterBG,
            BG_POSITION,
            rect
        );
    }

    for (unsigned int i = 0; i < text.size(); ++i) {
        // 描画位置を計算
        const GSvector2 position = { TEXT_POSITION.x, TEXT_POSITION.y + (FONT_SIZE + ROW_SPACE) * i };
        Canvas::draw_sprite_text(
            text[i],
            position,
            FONT_SIZE,
            cFONT,
            GS_FONT_NORMAL,
            GScolor{ 0.980f, 0.980f, 0.980f, 1.0f }
        );
    }
}

void TypeWriterLabel::clear() {
    type_writer_.clear();
}

void TypeWriterLabel::skip() {
    type_writer_.skip();
}

void TypeWriterLabel::set(const std::string& text, TextCode code) {
    type_writer_.set(text, code);
}

void TypeWriterLabel::set(const std::vector<std::string>& text, TextCode code) {
    type_writer_.set(text, code);
}
