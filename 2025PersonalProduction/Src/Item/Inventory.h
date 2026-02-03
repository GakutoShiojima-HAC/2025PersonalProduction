#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <unordered_map>
#include <string>
#include <vector>
#include "Item/ItemData.h"
#include "Weapon/WeaponData.h"
#include "Engine/Utils/MyJson.h"

class Inventory {
public:
    Inventory() = default;

    ~Inventory() = default;

public:
    /// <summary>
    /// インベントリをロード
    /// </summary>
    /// <param name="j">= jsonオブジェクト</param>
    void load(const json& j);

    /// <summary>
    /// セーブオブジェクトを取得
    /// </summary>
    /// <returns>ordered_jsonオブジェクト</returns>
    ordered_json save_object() const;

    /// <summary>
    /// インベントリをクリア
    /// </summary>
    void clear();

public:
    /// <summary>
    /// アイテムをインベントリに追加する
    /// </summary>
    /// <param name="type">= アイテムの種類</param>
    /// <param name="id">= アイテムのID</param>
    /// <param name="count">= アイテムの追加個数</param>
    /// <returns>追加できたら真を返却</returns>
    bool add(ItemType type, int id, int count = 1);

    /// <summary>
    /// アイテムをインベントリに追加する
    /// </summary>
    /// <param name="data">= アイテムデータ</param>
    /// <param name="count">= アイテムの追加個数</param>
    /// <returns>追加できたら真を返却</returns>
    bool add(ItemData::Data data, int count = 1);

    /// <summary>
    /// アイテムをインベントリから削除できるかどうか
    /// </summary>
    /// <param name="type">= アイテムの種類</param>
    /// <param name="id">= アイテムのID</param>
    /// <param name="count">= アイテムの削除個数</param>
    /// <param name="deleted">= そのまま削除するかどうか</param>
    /// <returns>削除できるなら真を返却</returns>
    bool can_remove(ItemType type, int id, int count, bool deleted = false);

    /// <summary>
    /// アイテムをインベントリから削除できるかどうか
    /// </summary>
    /// <param name="type">= アイテムの種類</param>
    /// <param name="count">= アイテムの削除個数</param>
    /// <param name="deleted">= そのまま削除するかどうか</param>
    /// <returns>削除できるなら真を返却</returns>
    bool can_remove(ItemData::Data data, int count, bool deleted = false);

    /// <summary>
    /// 現在の武器データを取得
    /// </summary>
    /// <returns>武器データ</returns>
    const WeaponData::Data& weapon() const;

    /// <summary>
    /// 武器を変える
    /// </summary>
    void change_weapon();

private:
    /// <summary>
    /// アイテムIDが範囲内かどうか
    /// </summary>
    /// <param name="type">= アイテムの種類</param>
    /// <param name="id">= アイテムID</param>
    /// <returns>範囲内なら真を返却</returns>
    bool is_in_id(ItemType type, int id) const;

    /// <summary>
    /// インベントリからアイテムを削除
    /// </summary>
    /// <param name="type">= アイテムの種類</param>
    /// <param name="id">= アイテムID</param>
    /// <param name="count">= アイテムの削除個数</param>
    void remove(ItemType type, int id, int count);

private:
    struct InventorySlot {
        int id{ 0 };
        int count{ 0 };

        InventorySlot() = default;
        InventorySlot(int id, int count) :
            id{ id },
            count{ count } {
        }
    };

private:
    std::unordered_map<ItemType, std::vector<InventorySlot>> inventory_;

    WeaponData::Data current_weapon_{ WeaponData::empty_data() };

public:
    // コピー禁止
    Inventory(const Inventory& other) = delete;
    Inventory& operator = (const Inventory& other) = delete;

};

#endif
