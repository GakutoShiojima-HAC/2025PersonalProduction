// -----------------------------------------------------------------------------------------
//  File          : WeaponType.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/06
//  Updated       : 2025/09/07
//  Description   : 武器の種類を定義
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef WEAPON_TYPE_H_
#define WEAPON_TYPE_H_

#define WEAPON_TYPE_LIST \
    X(NONE) \
    X(PlayerSword) \
    X(MAX_SIZE) // 要素数

#include <string>
#include <unordered_map>

typedef	unsigned int GSuint;
using namespace std;

#define X(name) name,
enum class WeaponType { WEAPON_TYPE_LIST };
#undef X

namespace Weapons {
    /// <summary>
    /// 文字列を武器番号に変換
    /// </summary>
    inline GSuint to_weapon_id(const string& name) {
        static const unordered_map<string, WeaponType> map = {
    #define X(name) {#name, WeaponType::name},
            WEAPON_TYPE_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)WeaponType::NONE;
    }
}

#endif