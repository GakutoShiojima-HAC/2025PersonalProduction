#include "Engine/Core/Button/ButtonManager.h"
#include "Engine/Core/Button/Button.h"

ButtonManager::~ButtonManager() {
    clear();
}

void ButtonManager::start() {
    select_index_ = 0;
}

void ButtonManager::update(float delta_time) {
    if (buttons_.empty()) return;

    // 入力したかどうか
    bool on_input = input_.action(InputAction::MENU_Decision);

    // ゲームパッド
    if (input_.is_pad()) {
        // 選択の更新
        update_select_index();

        for (unsigned int i = 0; i < buttons_.size(); ++i) {
            buttons_[i]->update(delta_time, i == select_index_);
            if (on_input) buttons_[i]->on_input_decision();
        }
    }
    // マウス
    else {
        const GSvector2 cursor_position = input_.cursor_position();
        for (auto& button : buttons_) {
            button->update(delta_time, cursor_position);
            if (on_input) button->on_input_decision();
        }
    }
}

void ButtonManager::draw() const {
    for (const auto& button : buttons_) {
        button->draw();
    }
}

void ButtonManager::clear() {
    for (auto button : buttons_) if (button != nullptr) delete button;
    buttons_.clear();
}

Button* ButtonManager::add(Button* button) {
    buttons_.push_back(button);
    return button;
}

void ButtonManager::update_select_index() {
    // 上入力があれば
    if (input_.action(InputAction::MENU_UP) && select_index_ > 0) select_index_ -= 1;
    // 下入力があれば
    if (input_.action(InputAction::MENU_DOWN) && select_index_ < buttons_.size() - 1) select_index_ += 1;
}
