#include "TypeWriter.h"
#include "GameConfig.h"
#include "Engine/Utils/MyString.h"

void TypeWriter::update(float delta_time) {
    if (state_ == State::Idle) return;

    // タイマーを進める
    timer_ -= delta_time / cFPS;
    if (timer_ > 0.0f) return;

    switch (state_) {
    case TypeWriter::State::Write:
        next_index();
        break;
    case TypeWriter::State::Display:
        state_ = State::Hide;
        timer_ = hide_time_;
        break;
    case TypeWriter::State::Hide:
        next_text();
        break;
    default:
        break;
    }
}

void TypeWriter::clear() {
    state_ = State::Idle;
    display_.clear();
    waiting_.clear();
    writing_ = "";
}

void TypeWriter::skip() {
    next_text();
}

void TypeWriter::set(const std::string& text, TextCode code) {
    waiting_.clear();
    waiting_.push_back(text);
    code_ = code;
    next_text();
}

void TypeWriter::set(const std::vector<std::string>& text, TextCode code) {
    waiting_ = text;
    code_ = code;
    next_text();
}

bool TypeWriter::is_empty() const {
    return state_ == State::Idle;
}

const std::vector<std::string>& TypeWriter::get() const {
    return display_;
}

void TypeWriter::next_text() {
    // 現在描画されているテキストを破棄
    display_.clear();

    // 待機文字列が無ければ終了
    if (waiting_.empty()) {
        state_ = State::Idle;
        return;
    }

    // 先頭の文字列を描画対象とする
    if (code_ == TextCode::UTF8) writing_ = MyString::utf8_to_shiftjis(waiting_.front());
    else writing_ = waiting_.front();
    waiting_.erase(waiting_.begin());
    state_ = State::Write;
    
    // 描画用変数をリセットする
    row_ = 0;
    cursor_ = 0;
    display_.push_back("");
    write_tempo_ = 0.1f;
    display_time_ = 1.0f;
    hide_time_ = 0.25f;
}

void TypeWriter::next_index() {
    auto next_state = [=]() {
        state_ = State::Display;
        timer_ = display_time_;
    };

    // 文字を全て描画変数に入れれたら次のステートへ
    if (cursor_ >= writing_.length()) {
        next_state();
        return;
    }

    // 行チェック
    if (display_.size() <= row_) {
        display_.push_back("");
    }

    // 文字を送ったかどうか？
    bool char_added = false;

    // 文字を送るまでループ
    while (cursor_ < writing_.length() && !char_added) {
        // タグの開始位置か？
        if (writing_[cursor_] == '<') {
            // タグの終了位置を探す
            size_t end_tag = writing_.find('>', cursor_);

            // タグであれば
            if (end_tag != std::string::npos) {
                // タグ文字列を取得
                std::string tag_content = writing_.substr(cursor_ + 1, end_tag - cursor_ - 1);
                apply_tag(tag_content);
                cursor_ = end_tag + 1;
                continue;   // タグ処理は文字送りに含まない
            }
        }

        // 2バイト文字かどうか？
        bool is_double_byte{ false };
        size_t char_length = 1;
        {
            unsigned char first_byte = static_cast<unsigned char>(writing_[cursor_]);
            is_double_byte = (first_byte >= 0x81 && first_byte <= 0x9F) || (first_byte >= 0xE0 && first_byte <= 0xFC);
        }
        if (is_double_byte) {
            // 文字列の末尾で途切れていないかチェック
            if (cursor_ + 1 < writing_.length()) {
                char_length = 2;
            }
        }

        // 文字を切り出す
        std::string sub = writing_.substr(cursor_, char_length);

        // 表示に追加
        display_[row_] += sub;
        cursor_ += char_length;

        char_added = true;  // 文字送りできたので終了
    }

    // 文字を全て描画変数に入れれたら次のステートへ
    if (cursor_ >= writing_.length()) {
        next_state();
        return;
    }

    // タイマーを再設定
    timer_ = write_tempo_;
}

void TypeWriter::apply_tag(const std::string& tag) {
    // 改行のタグ
    if(tag == "br") {
        row_++;
        display_.push_back(""); // 次の行を追加
        return;
    }

    // パラメータありのタグ
    size_t eq_pos = tag.find('=');
    if (eq_pos != std::string::npos) {
        std::string key = tag.substr(0, eq_pos);
        std::string val_str = tag.substr(eq_pos + 1);

        // 文字列をfloatに変換
        const float val = std::stof(val_str);

        if (key == "show") {
            display_time_ = val;
        }
        else if (key == "hide") {
            hide_time_ = val;
        }
        else if (key == "tempo") {
            write_tempo_ = val;
        }
    }
}

