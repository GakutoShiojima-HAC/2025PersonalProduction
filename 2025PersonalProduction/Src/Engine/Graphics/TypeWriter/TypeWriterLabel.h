// -----------------------------------------------------------------------------------------
//  File          : TypeWriterLabel.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/08
//  Updated       : 2025/12/08
//  Description   : 文字送りによるセリフを描画するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TYPE_WRITER_LABEL_H_
#define TYPE_WRITER_LABEL_H_

#include "TypeWriter.h"

class TypeWriterLabel {
public:
    TypeWriterLabel() = default;

    ~TypeWriterLabel() = default;

public:
    void update(float delta_time);

    void draw() const;

    void clear();

public:
    /// <summary>
    /// スキップ
    /// </summary>
    void skip();

    /// <summary>
    /// テキストを設定
    /// </summary>
    /// <param name="text">= テキスト</param>
    /// <param name="code">= 文字コード</param>
    void set(const std::string& text, TextCode code = TextCode::UTF8);

    /// <summary>
    /// テキストを負数設定
    /// </summary>
    /// <param name="text">= テキスト</param>
    /// <param name="code">= 文字コード</param>
    void set(const std::vector<std::string>& text, TextCode code = TextCode::UTF8);

private:
    TypeWriter type_writer_;

public:
    // コピー禁止
    TypeWriterLabel(const TypeWriterLabel& other) = delete;
    TypeWriterLabel& operator = (const TypeWriterLabel& other) = delete;

};


#endif
