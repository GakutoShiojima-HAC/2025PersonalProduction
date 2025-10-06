// -----------------------------------------------------------------------------------------
//  File          : WeaponData.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/02
//  Updated       : 2025/10/02
//  Description   : 武器データ
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef WEAPON_DATA_H_
#define WEAPON_DATA_H_

#define WEAPON_TYPE_LIST \
    X(NONE) \
    X(Sword) \
    X(Axe) \
    X(MAX_SIZE) // 要素数

#include <string>
#include <unordered_map>

#define X(name) name,
enum class WeaponType { WEAPON_TYPE_LIST };
#undef X

namespace WeaponData {
    /// <summary>
    /// 武器の空ID
    /// </summary>
    const int WEAPON_EMPTY_ID{ -999 };

    /// <summary>
    /// 武器データ構造体
    /// </summary>
    struct Data {
        int id{ 0 };
        std::string name{ "Weapon" };
        std::string description{ "Weapon." };
        GSuint texture{ 0 };
        GSuint mesh{ 0 };
        WeaponType type{ WeaponType::NONE };
        int damage{ 0 };

        /// <summary>
        /// 空かどうか
        /// </summary>
        /// <returns></returns>
        bool is_empty() const {
            return id == WEAPON_EMPTY_ID;
        }
    };

    /// <summary>
    /// 空のデータを作成して返却
    /// </summary>
    /// <returns>空のデータ</returns>
    inline WeaponData::Data empty_data() {
        WeaponData::Data data;
        data.id = WEAPON_EMPTY_ID;
        return data;
    }

    /// <summary>
    /// 文字列を武器番号に変換
    /// </summary>
    inline GSuint to_id(const std::string& name) {
        static const unordered_map<std::string, WeaponType> map = {
    #define X(name) { #name, WeaponType::name },
            WEAPON_TYPE_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)WeaponType::NONE;
    }

    /*
    /// <summary>
    /// 武器番号を文字列に変換
    /// </summary>
    inline const std::string& to_string(WeaponType type) {
        static const std::vector<std::string> names = {
    #define X(name) #name,
            WEAPON_TYPE_LIST
    #undef X
        };
        GSuint index = (GSuint)type;
        if (index < (GSuint)WeaponType::MAX_SIZE) return names[index];
        return names[(GSuint)WeaponType::NONE];
    }
    */
}

#endif
