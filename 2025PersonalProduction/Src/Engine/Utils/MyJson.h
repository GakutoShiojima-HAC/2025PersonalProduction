// -----------------------------------------------------------------------------------------
//  File          : MyJson.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/02
//  Updated       : 2025/11/07
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
    /// <param name="not_found_param">= 見つからなかったときのパラメータ</param>
    /// <returns>string</returns>
    inline std::string get_string(const json& json_object, const std::string& object_name, const std::string& not_found_param = "not found") {
        std::string result = not_found_param;
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_string())
            result = json_object[object_name.c_str()].get<std::string>();
        return result;
    };

    /// <summary>
    /// jsonオブジェクトからintを取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <param name="not_found_param">= 見つからなかったときのパラメータ</param>
    /// <returns>int</returns>
    inline int get_int(const json& json_object, const std::string& object_name, int not_found_param = -1) {
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_number_integer())
            return json_object[object_name.c_str()].get<int>();
        return not_found_param;
    };

    /// <summary>
    /// jsonオブジェクトからfloatを取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <param name="not_found_param">= 見つからなかったときのパラメータ</param>
    /// <returns>float</returns>
    inline float get_float(const json& json_object, const std::string& object_name, float not_found_param = -1.0f) {
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_number_float())
            return json_object[object_name.c_str()].get<float>();
        return not_found_param;
    }

    /// <summary>
    /// jsonオブジェクトからboolを取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <param name="not_found_param">= 見つからなかったときのパラメータ</param>
    /// <returns>bool</returns>
    inline bool get_boolean(const json& json_object, const std::string& object_name, bool not_found_param = false) {
        if (json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_boolean())
            return json_object[object_name.c_str()].get<bool>();
        return not_found_param;
    }

    /// <summary>
    /// jsonオブジェクトからvector3を取り出す
    /// </summary>
    /// <param name="json_object">= jsonオブジェクト</param>
    /// <param name="object_name">= オブジェクト名</param>
    /// <param name="not_found_param">= 見つからなかったときのパラメータ</param>
    /// <returns>GSvector3</returns>
    inline GSvector3 get_vector3(const json& json_object, const std::string& object_name, const GSvector3& not_found_param = GSvector3{ -1.0f, -1.0f, -1.0f }) {
        if (!json_object.contains(object_name) || !json_object[object_name].is_array() || json_object[object_name].size() != 3) return not_found_param;
        const auto& ary = json_object[object_name];
        if (!ary[0].is_number() || !ary[1].is_number() || !ary[2].is_number()) return not_found_param;
        return GSvector3{ ary[0].get<float>(), ary[1].get<float>(),ary[2].get<float>() };
    }
}

#endif
