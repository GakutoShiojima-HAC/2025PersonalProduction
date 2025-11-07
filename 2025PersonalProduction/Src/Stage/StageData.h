#ifndef STAGE_DATA_H_
#define STAGE_DATA_H_

#include <string>

struct StageDataStruct {
    // ステージ名
    std::string name{ "unknown" };
    // データのフォルダ
    std::string folder{ "" };

    // タイマーを使用するかどうか
    bool use_timer{ true };
    // ステージ終了後にリザルトを描画するかどうか
    bool use_result{ true };
};

class StageData {
public:
    StageData() = default;

    ~StageData() = default;

public:
    /// <summary>
    /// ステージデータを読み込む
    /// </summary>
    /// <param name="folder_path">= フォルダパス</param>
    void load(const std::string& folder_path);

public:
    /// <summary>
    /// ステージデータの取得
    /// </summary>
    /// <returns>ステージデータ</returns>
    StageDataStruct& data();

    /// <summary>
    /// ステージデータの取得(const)
    /// </summary>
    /// <returns>ステージデータ</returns>
    const StageDataStruct& data() const;

private:
    StageDataStruct data_;

public:
    // コピー禁止
    StageData(const StageData& other) = delete;
    StageData& operator = (const StageData& other) = delete;

};

#endif
