#include "GUI/Button/TextureFunctionButton.h"
#include <gslib.h>
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"

static const GScolor DEFAULT_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
static const GScolor HIGHLIGHT_COLOR{ 1.0f, 0.85f, 0.0f, 1.0f };

TextureFunctionButton::TextureFunctionButton(GSuint texture, const GSvector2& position, const GSrect& rect) {
    texture_ = texture;
    position_ = position;
    texture_rect_ = rect; 
    rect_ = GSrect{ rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y };
}

void TextureFunctionButton::draw() const {
    Canvas::draw_texture(texture_, position_, texture_rect_,
        GSvector2::zero(), GSvector2::one(), is_selected() ? HIGHLIGHT_COLOR : DEFAULT_COLOR);
}

void TextureFunctionButton::select() {

}

void TextureFunctionButton::state(float delta_time) {

}

void TextureFunctionButton::input() {
    if (input_func_) input_func_();
}

void TextureFunctionButton::exit() {

}

void TextureFunctionButton::on_input(std::function<void()> input_func) {
    input_func_ = input_func;
}
