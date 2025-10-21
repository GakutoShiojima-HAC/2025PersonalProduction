#include "Engine/Core/Button/Button.h"
#include <gslib.h>

void Button::update(float delta_time, bool is_selected) {
    // 今回が初めてなら更新
    if (is_selected && !is_selected_) {
        is_selected_ = true;
        select();
    }
    // 前回まで選択中なら更新
    if (!is_selected && is_selected_) {
        is_selected_ = false;
        exit();
    }

    state(delta_time);
}

void Button::update(float delta_time, const GSvector2& cursor_position) {
    // ボタン判定内に入っているかどうか
    bool is_selected = cursor_position.x >= rect_.left && cursor_position.x <= rect_.right && cursor_position.y >= rect_.top && cursor_position.y <= rect_.bottom;

    // 今回が初めてなら更新
    if (is_selected && !is_selected_) {
        is_selected_ = true;
        select();
    }
    // 前回まで選択中なら更新
    if (!is_selected && is_selected_) {
        is_selected_ = false;
        exit();
    }

    state(delta_time);
}

void Button::on_input_decision() {
    // 選択中のボタンなら呼び出す
    if (is_selected_) input();
}

bool Button::is_selected() const {
    return is_selected_;
}
