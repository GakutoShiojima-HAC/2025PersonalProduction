// -----------------------------------------------------------------------------------------
//  File          : WeaponDataManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/02
//  Updated       : 2025/10/02
//  Description   : 武器のパラメータを管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef WEAPON_DATA_MANAGER_H_
#define WEAPON_DATA_MANAGER_H_

#include <string>
#include <unordered_map>
#include "Item/Weapon/WeaponData.h"

class WeaponDataManager {
public:
    WeaponDataManager() = default;

    ~WeaponDataManager();

public:
    /// <summary>
    /// 武器データを読み込む
    /// </summary>
    /// <param name="folder_path">フォルダパス</param>
    void load(const std::string& folder_path);

    /// <summary>
    /// 武器データをクリア
    /// </summary>
    void clear();

public:
    /// <summary>
    /// 武器データを取得
    /// </summary>
    /// <param name="id">= 武器ID</param>
    /// <returns>武器データ</returns>
    WeaponData::Data get(int id) const;

    /// <summary>
    /// 武器データのバージョンを取得
    /// </summary>
    /// <returns>バージョン番号</returns>
    int version() const;

private:
    std::unordered_map<int, WeaponData::Data> data_;

    int version_{ -1 };

public:
    // コピー禁止
    WeaponDataManager(const WeaponDataManager& other) = delete;
    WeaponDataManager& operator = (const WeaponDataManager& other) = delete;

};

#endif
