// -----------------------------------------------------------------------------------------
//  File          : ItemDataManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/02
//  Updated       : 2025/10/02
//  Description   : アイテムデータを読み込み、返却する管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ITEM_DATA_MANAGER_H_
#define ITEM_DATA_MANAGER_H_

#include "Item/ItemData.h"
#include "Item/Weapon/WeaponDataManager.h"

class ItemDataManager {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    ItemDataManager() = default;

public:
    ~ItemDataManager() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static ItemDataManager& get_instance();

public:
    /// <summary>
    /// すべてのアイテム情報を読み込む
    /// </summary>
    void load();

    /// <summary>
    /// アイテム名を取得
    /// </summary>
    /// <param name="data">= アイテムデータ</param>
    /// <returns>アイテム名</returns>
    std::string get_name(ItemData::Data data) const;

    /// <summary>
    /// アイテムが存在するかどうか
    /// </summary>
    /// <param name="data">= アイテムデータ</param>
    /// <returns>存在するなら真を返却</returns>
    bool is_empty(ItemData::Data data) const;

    /// <summary>
    /// 武器データを取得
    /// </summary>
    /// <param name="id">= 武器ID</param>
    /// <returns>武器データ</returns>
    WeaponData::Data get_weapon(int id) const;

    /// <summary>
    /// 武器データのバージョン
    /// </summary>
    /// <returns>バージョン番号</returns>
    int weapon_version() const;

private:
    WeaponDataManager weapon_data_;

public:
    // コピー禁止
    ItemDataManager(const ItemDataManager& other) = delete;
    ItemDataManager& operator = (const ItemDataManager& other) = delete;

};

#endif
