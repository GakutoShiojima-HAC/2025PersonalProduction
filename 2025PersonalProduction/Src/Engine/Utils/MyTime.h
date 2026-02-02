// -----------------------------------------------------------------------------------------
//  File        : MyTime.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/09/15
//  Description : 時間用便利関数群
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MY_TIME_H_
#define MY_TIME_H_

#include <string>
#include <iomanip>  // std::setw, std::setfill
#include <sstream>  // std::ostringstream
#include <ctime>    // std::time_t, std::tm, std::localtime

using namespace std;

namespace MyLib {
#pragma region [Time]
    /// <summary>
    /// 日時情報構造体
    /// </summary>
    struct TimeInfo {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        /// <summary>
        /// "YYYY/MM/DD HH:MM::SS" の形式で日時情報を文字列に変換したものを返却
        /// </summary>
        string time_info_to_string() const {
            ostringstream oss;
            oss << setfill('0')  // 0埋めする
                << setw(4) << year << "/"
                << setw(2) << month << "/"
                << setw(2) << day << " "
                << setw(2) << hour << ":"
                << setw(2) << minute << ":"
                << setw(2) << second;
            return oss.str();
        }

        /// <summary>
        /// "YYYY_MM_DD_HHMM" の形式で日時情報を文字列に変換したものを返却
        /// </summary>
        string time_info_to_filename_string() const {
            ostringstream oss;
            oss << setfill('0')  // 0埋め
                << setw(4) << year << "_"
                << setw(2) << month << "_"
                << setw(2) << day << "_"
                << setw(2) << hour
                << setw(2) << minute;
            return oss.str();
        }
    };

    /// <summary>
    /// 現在の日時情報を返却
    /// </summary>
    inline TimeInfo get_time_info() {
        time_t current = time(nullptr);    // 現在のUNIX時間
        tm local;                          // 安全なローカル時間構造体
        localtime_s(&local, &current);     // 安全な変換

        TimeInfo result;
        result.year = local.tm_year + 1900;
        result.month = local.tm_mon + 1;
        result.day = local.tm_mday;
        result.hour = local.tm_hour;
        result.minute = local.tm_min;
        result.second = local.tm_sec;

        return result;
    }
#pragma endregion

}

#endif MY_TIME_H_
