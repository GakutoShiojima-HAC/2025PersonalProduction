// -----------------------------------------------------------------------------------------
//  File          : CameraTimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : カメラ用タイムラインパラメータ
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_TIMELINE_PARAMETER_H_
#define CAMERA_TIMELINE_PARAMETER_H_

#include "ITimelineParameter.h"
#include <gslib.h>
#include <vector>

class Camera;

/// <summary>
/// キーフレーム
/// </summary>
struct CameraTimelineKeyFrame {
    CameraTimelineKeyFrame(
        float time,
        const std::string& target,
        GSvector3 position,
        GSvector3 lookat,
        float angle
    ) :
        time{ time },
        target{ target },
        position{ position },
        lookat{ lookat },
        angle{ angle }
    {}

    float time{ 0.0f };						// 時間(秒)
    std::string target{ "" };				// 基点とするアクター(存在しなければワールド原点を基点とする)
    GSvector3 position{ 0.0f, 0.0f, 0.0f };	// 基点からの相対座標
    GSvector3 lookat{ 0.0f, 0.0f, 0.0f };	// 注視点
    float angle{ 0.0f };					// 角度
};

class CameraTimelineParameter : public ITimelineParameter {
public:
    CameraTimelineParameter(World* world);

    ~CameraTimelineParameter();

public:
    void update(float delta_time) override;

    void clear() override;

    std::string name() const override;

    void load(const std::string& name, const json& j) override;

    void play(const std::string& name) override;

    void stop() override;

private:
    // タイムライン用カメラ
    Camera* camera_{ nullptr };
    // タイムライン用カメラが有効になる前のカメラ
    Camera* prev_camera_{ nullptr };

#pragma region [キーフレーム管理を行うタイムラインデータクラス]
public:
    /// <summary>
    /// キーフレーム管理を行うタイムラインデータクラス
    /// </summary>
    class CameraTimelineData {
    public:
        CameraTimelineData(const std::vector<CameraTimelineKeyFrame*>& timeline, float start_transition_time = 0.0f, float end_transition_time = 0.0f);

        ~CameraTimelineData();

    public:
        void clear();

    public:
        /// <summary>
        /// タイムラインデータ
        /// </summary>
        std::vector<CameraTimelineKeyFrame*>& get();

        /// <summary>
        /// タイムライン用カメラに遷移するまでの時間
        /// </summary>
        float& start_transition_time();

        /// <summary>
        /// 元のカメラに遷移するまでの時間
        /// </summary>
        float& end_transition_time();

    private:
        // タイムラインデータ
        std::vector<CameraTimelineKeyFrame*> timeline_;
        // タイムライン用カメラに遷移するまでの時間
        float start_transition_time_{ 0.0f };
        // 元のカメラに遷移するまでの時間
        float end_transition_time_{ 0.0f };

    public:
        // コピー禁止
        CameraTimelineData(const CameraTimelineData& other) = delete;
        CameraTimelineData& operator = (const CameraTimelineData& other) = delete;

    };
#pragma endregion

public:
    /// <summary>
    /// タイムラインデータを再生する
    /// </summary>
    /// <param name="data">= タイムラインデータ メモリ解放はしません</param>
    void play(CameraTimelineData* data);

    /// <summary>
    /// データを生成
    /// </summary>
    /// <param name="j">= データオブジェクト</param>
    /// <returns>データ</returns>
    CameraTimelineData* create_data(const json& j);

private:
    /// <summary>
    /// タイムラインデータを管理下に追加
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    /// <param name="data">= データ</param>
    void add(const string& name, CameraTimelineData* data);

    /// <summary>
    /// タイムラインデータを検索
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    CameraTimelineData* find(const std::string& name);

private:
    // タイムラインデータマップ
    std::unordered_map<std::string, CameraTimelineData*> timelines_;
    // 現在再生中のタイムラインデータ
    CameraTimelineData* current_{ nullptr };

};

#endif
