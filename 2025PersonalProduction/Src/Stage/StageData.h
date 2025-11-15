#ifndef STAGE_DATA_H_
#define STAGE_DATA_H_

#include <string>
#include <gslib.h>

/// <summary>
/// ロード時に使用するステージ構成
/// </summary>
struct StageLoadConfigData {
    // ナビメッシュパス
    std::string navmesh{ "" };
    // ライトマップパス
    std::string lightmap{ "" };
    // リフレクションプローブパス
    std::string refprobe{ "" };

};

/// <summary>
/// ステージ構成
/// </summary>
struct StageConfigData {
    // ステージ名
    std::string name{ "unknown" };
    // データのフォルダ
    std::string folder{ "" };

    // ライトの角度(Unity基準)
    GSvector3 light_angle{ 0.0f, 0.0f, 0.0f };

    // タイマーを使用するかどうか
    bool use_timer{ true };
    // ステージ終了後にリザルトを描画するかどうか
    bool use_result{ true };
    // 通常エネミーのカウンターとシネマイベントを生成するかどうか
    bool use_normal_enemy_counter{ true };
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
    /// <returns>ロード時に使用するステージ構成</returns>
    StageLoadConfigData load(const std::string& folder_path);

public:
    /// <summary>
    /// ステージデータの取得
    /// </summary>
    /// <returns>ステージデータ</returns>
    StageConfigData& data();

    /// <summary>
    /// ステージデータの取得(const)
    /// </summary>
    /// <returns>ステージデータ</returns>
    const StageConfigData& data() const;

private:
    StageConfigData data_;

public:
    // コピー禁止
    StageData(const StageData& other) = delete;
    StageData& operator = (const StageData& other) = delete;

};

#endif
