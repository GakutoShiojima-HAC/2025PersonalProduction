#include "GUI/Button/TextureFunctionButton.h"
#include <gslib.h>
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"

static const GScolor DEFAULT_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
static const GScolor HIGHLIGHT_COLOR{ 1.0f, 0.85f, 0.0f, 1.0f };

TextureFunctionButton::TextureFunctionButton(GSuint texture, const GSvector2& position, const GSrect& rect, Angle angle) {
    texture_ = texture;
    position_ = position;
    texture_rect_ = rect;

    const float width = rect.right - rect.left;
    const float height = rect.bottom - rect.top;
    switch (angle) {
    case TextureFunctionButton::Angle::Rotate90:angle_ = 90.0f;
        rect_ = GSrect{ position.x - height, position.y, position.x, position.y + width };
        break;
    case TextureFunctionButton::Angle::Rotate180:angle_ = 180.0f;
        rect_ = GSrect{ position.x - width, position.y - height, position.x, position.y };
        break;
    case TextureFunctionButton::Angle::Rotate270:angle_ = 270.0f;
        rect_ = GSrect{ position.x, position.y - width, position.x + height, position.y };
        break;
    case TextureFunctionButton::Angle::Rotate0: 
    default: angle_ = 0.0f;
        rect_ = GSrect{ position.x, position.y, position.x + width, position.y + height };
        break;
    }
}

void TextureFunctionButton::draw() const {
    Canvas::draw_texture(texture_, position_, texture_rect_,
        GSvector2::zero(), GSvector2::one(), is_selected() ? HIGHLIGHT_COLOR : DEFAULT_COLOR, angle_);
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
