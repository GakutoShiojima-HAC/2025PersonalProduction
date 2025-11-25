// -----------------------------------------------------------------------------------------
//  File          : PlayBGMTimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/26
//  Updated       : 2025/11/26
//  Description   : BGMを再生
//  
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYBGM_TIMELINE_PARAMETER_H_
#define PLAYBGM_TIMELINE_PARAMETER_H_

#include "ITimelineParameter.h"
#include <vector>
#include "Engine/Sound/BGMManager.h"

/// <summary>
/// キーフレーム
/// </summary>
struct PlayBGMTimelineKeyFrame {
    PlayBGMTimelineKeyFrame(
        float time,
        const std::string& bgm_name,
        float transition_time
    ) :
        time{ time },
        bgm_name{ bgm_name },
        transition_time{ transition_time }
    {
    }

    float time{ 0.0f };						// 時間(秒)
    std::string bgm_name{ "" };             // 再生するBGM
    float transition_time{ 0.0f };          // フェードにかかる時間(秒)
};  

class PlayBGMTimelineParameter : public ITimelineParameter {
public:
    PlayBGMTimelineParameter(World* world);

    ~PlayBGMTimelineParameter();

public:
    void update(float delta_time) override;

    void clear() override;

    std::string name() const override;

    void load(const std::string& name, const json& j) override;

    void play(const std::string& name) override;

    void stop() override;

#pragma region [キーフレーム管理を行うタイムラインデータクラス]
public:
    /// <summary>
    /// キーフレーム管理を行うタイムラインデータクラス
    /// </summary>
    class PlayBGMTimelineData {
    public:
        PlayBGMTimelineData(const std::vector<PlayBGMTimelineKeyFrame*>& timeline);

        ~PlayBGMTimelineData();

    public:
        void clear();

    public:
        /// <summary>
        /// タイムラインデータ
        /// </summary>
        std::vector<PlayBGMTimelineKeyFrame*>& get();

    private:
        // タイムラインデータ
        std::vector<PlayBGMTimelineKeyFrame*> timeline_;
 
    public:
        // コピー禁止
        PlayBGMTimelineData(const PlayBGMTimelineData& other) = delete;
        PlayBGMTimelineData& operator = (const PlayBGMTimelineData& other) = delete;

    };
#pragma endregion

public:
    /// <summary>
    /// タイムラインデータを再生する
    /// </summary>
    /// <param name="data">= タイムラインデータ メモリ解放はしません</param>
    void play(PlayBGMTimelineData* data);

    /// <summary>
    /// データを生成
    /// </summary>
    /// <param name="j">= データオブジェクト</param>
    /// <returns>データ</returns>
    PlayBGMTimelineData* create_data(const json& j);

private:
    /// <summary>
    /// タイムラインデータを管理下に追加
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    /// <param name="data">= データ</param>
    void add(const string& name, PlayBGMTimelineData* data);

    /// <summary>
    /// タイムラインデータを検索
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    PlayBGMTimelineData* find(const std::string& name);

private:
    // タイムラインデータマップ
    std::unordered_map<std::string, PlayBGMTimelineData*> timelines_;
    // 現在再生中のタイムラインデータ
    PlayBGMTimelineData* current_{ nullptr };
    // BGMマネージャー
    BGMManager& manager_ = BGMManager::get_instance();

};

#endif
