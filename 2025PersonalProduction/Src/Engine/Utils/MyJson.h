// -----------------------------------------------------------------------------------------
//  File          : MyJson.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/02
//  Updated       : 2025/10/03
//  Description   : nlohmann jsonをさらに便利にする関数
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_MYJSON_H_
#define MYLIB_MYJSON_H_

#include <string>
#include <fstream>
#include "Lib/json.hpp"
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace MyJson {
    /// <summary>
    /// 指定したファイルがjsonファイルかどうか
    /// jsonファイルならjsonクラスにjsonファイルの内容を読み込む
    /// </summary>
    /// <param name="file_path">= ファイルパス</param>
    /// <param name="j">= jsonクラス</param>
    /// <returns>jsonファイルなら真を返却</returns>
    inline bool is_json(const std::string& file_path, json& j) {
        std::ifstream file(file_path);
        if (!file.is_open()) return false;
        file >> j;
        return true;
    };

    /// <summary>
    /// 指定した名前を持つjsonオブジェクトかどうか
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <returns>jsonオブジェクトであるなら真を返却</returns>
    inline bool is_object(const json& json_object, const std::string& object_name) {
        return json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_object();
    };

    /// <summary>
    /// jsonオブジェクトからstringを取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <returns>string</returns>
    inline std::string get_string(const json& json_object, const std::string& object_name) {
        std::string result = "none";
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_string())
            result = json_object[object_name.c_str()].get<std::string>();
        return result;
    };

    /// <summary>
    /// jsonオブジェクトからintを取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <returns>int</returns>
    inline int get_int(const json& json_object, const std::string& object_name) {
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_number_integer())
            return json_object[object_name.c_str()].get<int>();
        return -1;
    };

    /// <summary>
    /// jsonオブジェクトからfloatを取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <returns>float</returns>
    inline float get_float(const json& json_object, const std::string& object_name) {
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_number_float())
            return json_object[object_name.c_str()].get<float>();
        return -1.0f;
    }
}

#endif
