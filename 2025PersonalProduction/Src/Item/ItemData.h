// -----------------------------------------------------------------------------------------
//  File          : ItemData.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/30
//  Updated       : 2025/10/02
//  Description   : アイテムデータ
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ITEM_DATA_H_
#define ITEM_DATA_H_

#define ITEM_TYPE_LIST \
    X(NONE) \
    X(Weapon) \
    X(MAX_SIZE) // 要素数

#include <string>
#include <unordered_map>

#define X(name) name,
enum class ItemType { ITEM_TYPE_LIST };
#undef X

namespace ItemData {
    /// <summary>
    ///  アイテムのNULLID
    /// </summary>
    const int ITEM_NULL_ID{ -999 };

    struct Data {
        // アイテムの種類
        ItemType type{ ItemType::NONE };
        // アイテムのID
        int id{ 0 };

        Data() = default;
        Data(ItemType type, int id) :
            type{ type },
            id{ id } {
        }
    };

    /// <summary>
    /// 文字列をアイテムの種類に変換
    /// </summary>
    inline ItemType to_type(const std::string& name) {
        static const unordered_map<std::string, ItemType> map = {
    #define X(name) { #name, ItemType::name },
            ITEM_TYPE_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? it->second : ItemType::NONE;
    }

    /// <summary>
    /// アイテムの種類を文字列に変換
    /// </summary>
    inline const std::string& to_string(ItemType type) {
        static const std::vector<std::string> names = {
    #define X(name) #name,
            ITEM_TYPE_LIST
    #undef X
        };
        GSuint index = (GSuint)type;
        if (index < (GSuint)ItemType::MAX_SIZE) return names[index];
        return names[(GSuint)ItemType::NONE];
    }
}

#endif
