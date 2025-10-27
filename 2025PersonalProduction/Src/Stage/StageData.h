#ifndef STAGE_DATA_H_
#define STAGE_DATA_H_

#include <string>

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
    /// ステージ名を取得
    /// </summary>
    /// <returns>ステージ名(UTF-8)</returns>
    std::string name() const;

    /// <summary>
    /// フォルダ名を取得
    /// </summary>
    /// <returns></returns>
    std::string folder() const;

private:
    // ステージ名
    std::string name_{ "unknown" };
    // データのフォルダ
    std::string folder_{ "" };

public:
    // コピー禁止
    StageData(const StageData& other) = delete;
    StageData& operator = (const StageData& other) = delete;

};

#endif
