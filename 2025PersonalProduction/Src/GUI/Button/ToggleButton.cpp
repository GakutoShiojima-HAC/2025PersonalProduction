#include "GUI/Button/ToggleButton.h"

ToggleButton::ToggleButton(bool& toggle, const GSvector2& position, GSuint font_size, const Anchor anchor, const Anchor text_anchor) :
    toggle_{ toggle },
    TextFunctionButton("Toggle", position, font_size, anchor, text_anchor)
{
    change_text(toggle ? "ON" : "OFF");
}

void ToggleButton::select() {

}

void ToggleButton::state(float delta_time) {

}

void ToggleButton::input() {
	toggle_ = !toggle_;	// toggle‚È‚Ì‚Å”½“]
	change_text(toggle_ ? "ON" : "OFF");
	if (input_func_) input_func_();
}

void ToggleButton::exit() {

}
