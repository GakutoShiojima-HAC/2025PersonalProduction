// -----------------------------------------------------------------------------------------
//  File          : SendMessageTimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/15
//  Updated       : 2025/11/15
//  Description   : アクターにメッセージを送れるタイムラインパラメータ
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SEND_MESSAGE_TIMELINE_PARAMETER_H_
#define SEND_MESSAGE_TIMELINE_PARAMETER_H_

#include "ITimelineParameter.h"
#include <vector>

/// <summary>
/// キーフレーム
/// </summary>
struct SendMessageTimelineKeyFrame {
    SendMessageTimelineKeyFrame(
        float time,
        const std::string& target,
        const std::string& message
    ) :
        time{ time },
        target{ target },
        message{ message }
    {
    }

    float time{ 0.0f };						// 時間(秒)
    std::string target{ "" };				// メッセージを送るアクター
    std::string message{ "" };              // メッセージ内容
};

class SendMessageTimelineParameter : public ITimelineParameter {
public:
    SendMessageTimelineParameter(World* world);

    ~SendMessageTimelineParameter();

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
    class SendMessageTimelineData {
    public:
        SendMessageTimelineData(const std::vector<SendMessageTimelineKeyFrame*>& timeline);

        ~SendMessageTimelineData();

    public:
        void clear();

    public:
        /// <summary>
        /// タイムラインデータ
        /// </summary>
        std::vector<SendMessageTimelineKeyFrame*>& get();

    private:
        // タイムラインデータ
        std::vector<SendMessageTimelineKeyFrame*> timeline_;
 
    public:
        // コピー禁止
        SendMessageTimelineData(const SendMessageTimelineData& other) = delete;
        SendMessageTimelineData& operator = (const SendMessageTimelineData& other) = delete;

    };
#pragma endregion

public:
    /// <summary>
    /// タイムラインデータを再生する
    /// </summary>
    /// <param name="data">= タイムラインデータ メモリ解放はしません</param>
    void play(SendMessageTimelineData* data);

    /// <summary>
    /// データを生成
    /// </summary>
    /// <param name="j">= データオブジェクト</param>
    /// <returns>データ</returns>
    SendMessageTimelineData* create_data(const json& j);

private:
    /// <summary>
    /// タイムラインデータを管理下に追加
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    /// <param name="data">= データ</param>
    void add(const string& name, SendMessageTimelineData* data);

    /// <summary>
    /// タイムラインデータを検索
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    SendMessageTimelineData* find(const std::string& name);

private:
    // タイムラインデータマップ
    std::unordered_map<std::string, SendMessageTimelineData*> timelines_;
    // 現在再生中のタイムラインデータ
    SendMessageTimelineData* current_{ nullptr };

};

#endif
