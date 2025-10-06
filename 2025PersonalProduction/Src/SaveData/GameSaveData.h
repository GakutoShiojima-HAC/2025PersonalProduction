#ifndef SAVE_DATA_H_
#define SAVE_DATA_H_

#include <string>
#include "Item/Inventory.h"

class GameSaveData {
public:
    GameSaveData() = default;

    ~GameSaveData() = default;

public:
    /// <summary>
    /// ゲームに関係のあるデータを一括で読み込む
    /// </summary>
    /// <param name="folder_path">フォルダパス</param>
    void load(const std::string& folder_path);

    /// <summary>
    /// ゲームに関係のあるデータを一括で保存する
    /// </summary>
    void save();

    /// <summary>
    /// ゲームに関係のあるデータを一括でクリアする
    /// </summary>
    void clear();

public:
    /// <summary>
    /// プレイヤーのインベントリを取得
    /// </summary>
    /// <returns>インベントリ</returns>
    Inventory& inventory();

private:
    // セーブ先のフォルダ
    std::string save_folder_path_{ "" };

private:
    // プレイヤーインベントリ
    Inventory inventory_;

public:
    // コピー禁止
    GameSaveData(const GameSaveData& other) = delete;
    GameSaveData& operator = (const GameSaveData& other) = delete;

};
#endif
