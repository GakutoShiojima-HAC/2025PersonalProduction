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
    /// utf8のstringをshiftjisに変換
    /// </summary>
    /// <param name="value">= utf8の文字列</param>
    /// <returns>shiftjisの文字列</returns>
    inline std::string utf8_to_shiftjis(const std::string& value) {
        // UTF-8をWide文字列に変換
        int wlen = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, nullptr, 0);
        std::vector<wchar_t> wide_text(wlen);
        MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wide_text.data(), wlen);

        // Wide文字列をShift-JISに変換
        int sjis = WideCharToMultiByte(932, 0, wide_text.data(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> sjis_text(sjis);
        WideCharToMultiByte(932, 0, wide_text.data(), -1, sjis_text.data(), sjis, nullptr, nullptr);

        return std::string(sjis_text.data());
    }

    /// <summary>
    /// shiftjisのstringをutf8に変換
    /// </summary>
    /// <param name="value">= shiftjisの文字列</param>
    /// <returns>utf8の文字列</returns>
    inline std::string shiftjis_to_utf8(const std::string& value) {
        // Shift-JISをwide文字列に変換
        int wlen = MultiByteToWideChar(932, 0, value.c_str(), -1, nullptr, 0);
        std::wstring wide_text(wlen, 0);
        MultiByteToWideChar(932, 0, value.c_str(), -1, &wide_text[0], wlen);

        // Wide文字列をUTF-8に変換
        int utf8len = WideCharToMultiByte(CP_UTF8, 0, wide_text.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string utf8(utf8len, 0);
        WideCharToMultiByte(CP_UTF8, 0, wide_text.c_str(), -1, &utf8[0], utf8len, nullptr, nullptr);
        if (!utf8.empty() && utf8.back() == '\0')utf8.pop_back();

        return utf8;
    }
}

inline std::string ToUTF8(const std::string& sjis) { return MyString::shiftjis_to_utf8(sjis); }

inline std::string ToSJIS(const std::string& utf8) { return MyString::utf8_to_shiftjis(utf8); }

#endif
