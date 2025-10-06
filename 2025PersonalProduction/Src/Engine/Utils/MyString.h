// -----------------------------------------------------------------------------------------
//  File          : MyString.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/03
//  Updated       : 2025/10/03
//  Description   : string関連便利クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_MY_STRING_H_
#define MYLIB_MY_STRING_H_

#include <string>

namespace MyString {
    /// <summary>
    /// utf8のstringをshiftjisで取得
    /// </summary>
    /// <param name="value">= utf8の文字列</param>
    /// <returns>shiftjisの文字列</returns>
    std::string utf8_to_shiftjis(const std::string& value) {
        // UTF-8をWide文字列に変換
        int wlen = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, nullptr, 0);
        std::vector<wchar_t> wide_text(wlen);
        MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wide_text.data(), wlen);

        // Wide文字列をShift-JISに変換
        int sjis = WideCharToMultiByte(932, 0, wide_text.data(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> sjis_text(sjis);
        WideCharToMultiByte(932, 0, wide_text.data(), -1, sjis_text.data(), sjis, nullptr, nullptr);

        return std::string(sjis_text.begin(), sjis_text.end());
    }
}

#endif
