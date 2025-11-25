#ifndef SAVE_DATA_H_
#define SAVE_DATA_H_

#include <string>
#include <vector>
#include "Item/Inventory.h"

struct SaveData {
    int stage{ -1 };    // -1でチュートリアル未クリア 0以上ならindex + 1のステージに挑戦できる
    int player_level{ 1 };
};

class GameSaveData {
public:
    GameSaveData() = default;

    ~GameSaveData() = default;

public:
    /// <summary>
    /// ゲームに関係のあるデータを読み込む
    /// </summary>
    /// <param name="file_path">ファイルパス</param>
    void load(const std::string& file_path);

    /// <summary>
    /// ゲームに関係のあるデータを保存する
    /// </summary>
    void save();

    /// <summary>
    /// ゲームに関係のあるデータをクリアする
    /// </summary>
    void clear();

public:
    /// <summary>
    /// プレイヤーのインベントリを取得
    /// </summary>
    /// <returns>インベントリ</returns>
    Inventory& inventory();

    /// <summary>
    /// セーブデータの取得(const)
    /// </summary>
    /// <returns></returns>
    const SaveData& get() const;

    /// <summary>
    /// 全てのセーブデータファイルを取得
    /// </summary>
    /// <returns>全てのセーブデータファイル</returns>
    std::vector<string> get_all_save_file() const;

public:
    /// <summary>
    /// クリアしたステージ番号を保存
    /// </summary>
    /// <param name="stage_num">= クリアしたステージ番号</param>
    void set_clear_stage(int stage_num);

private:
    // 使用中のセーブデータファイル
    std::string save_file_path_{ "" };

private:
    // セーブデータ
    SaveData save_data_;
    // プレイヤーインベントリ
    Inventory inventory_;
    
public:
    // コピー禁止
    GameSaveData(const GameSaveData& other) = delete;
    GameSaveData& operator = (const GameSaveData& other) = delete;

};
#endif
