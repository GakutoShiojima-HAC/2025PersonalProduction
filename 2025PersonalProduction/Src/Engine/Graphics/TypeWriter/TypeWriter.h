// -----------------------------------------------------------------------------------------
//  File          : TypeWriter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/08
//  Updated       : 2025/12/08
//  Description   : 文字送りによるセリフを管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TYPE_WRITER_H_
#define TYPE_WRITER_H_

#include <string>
#include <vector>
#include "TextCode.h"

class TypeWriter {
public:
    TypeWriter() = default;

    ~TypeWriter() = default;

public:
    void update(float delta_time);

    void clear();

    void skip();

    void set(const std::string& text, TextCode code = TextCode::UTF8);

    void set(const std::vector<std::string>& text, TextCode code = TextCode::UTF8);

    bool is_empty() const;

    const std::vector<std::string>& get() const;

private:
    /// <summary>
    /// 次のテキストを描画する
    /// </summary>
    void next_text();

    /// <summary>
    /// 次の文字を描画する
    /// </summary>
    void next_index();

    /// <summary>
    /// タグを適用する
    /// </summary>
    /// <param name="tag">= タグ文字列</param>
    void apply_tag(const std::string& tag);

private:
    enum class State {
        Idle,       // 待機（表示する文字列待ち）
        Write,      // 文字送り中
        Display,    // 表示中(文字送りが終わって全体を表示)
        Hide        // 非表示中(次の文字送りまでの待機)
    };

private:
    // 現在の状態
    State state_{ State::Idle };
    // 現在の文字コード
    TextCode code_{ TextCode::UTF8 };

    // 待機中の文字列
    std::vector<std::string> waiting_;
    // 文字送り中の文字列
    std::string writing_{ "" };
    // 描画する文字列
    std::vector<std::string> display_;
    // どこまで読んだか(バイト数)
    size_t cursor_{ 0 };
    // 行
    unsigned int row_{ 0 };

    // タイマー
    float timer_{ 0.0f };

    // 文字送りの間隔
    float write_tempo_{ 0.1f };
    // 描画する時間
    float display_time_{ 1.0f };
    // 非表示する時間
    float hide_time_{ 0.25f };

public:
    // コピー禁止
    TypeWriter(const TypeWriter& other) = delete;
    TypeWriter& operator = (const TypeWriter& other) = delete;

};


#endif
