// -----------------------------------------------------------------------------------------
//  File          : TypeWriterTimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : タイプライターのパラメータ
//  
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TYPEWRITER_TIMELINE_PARAMETER_H_
#define TYPEWRITER_TIMELINE_PARAMETER_H_

#include "ITimelineParameter.h"
#include <vector>

/// <summary>
/// キーフレーム
/// </summary>
struct TypeWriterTimelineKeyFrame {
    TypeWriterTimelineKeyFrame(
        float time,
        const std::vector<std::string>& text
    ) :
        time{ time },
        text{ text }
    {
    }

    float time{ 0.0f };						// 時間(秒)
    std::vector<std::string> text;          // 描画テキスト
};

class TypeWriterTimelineParameter : public ITimelineParameter {
public:
    TypeWriterTimelineParameter(World* world);

    ~TypeWriterTimelineParameter();

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
    class TypeWriterTimelineData {
    public:
        TypeWriterTimelineData(const std::vector<TypeWriterTimelineKeyFrame*>& timeline);

        ~TypeWriterTimelineData();

    public:
        void clear();

    public:
        /// <summary>
        /// タイムラインデータ
        /// </summary>
        std::vector<TypeWriterTimelineKeyFrame*>& get();

    private:
        // タイムラインデータ
        std::vector<TypeWriterTimelineKeyFrame*> timeline_;
 
    public:
        // コピー禁止
        TypeWriterTimelineData(const TypeWriterTimelineData& other) = delete;
        TypeWriterTimelineData& operator = (const TypeWriterTimelineData& other) = delete;

    };
#pragma endregion

public:
    /// <summary>
    /// タイムラインデータを再生する
    /// </summary>
    /// <param name="data">= タイムラインデータ メモリ解放はしません</param>
    void play(TypeWriterTimelineData* data);

    /// <summary>
    /// データを生成
    /// </summary>
    /// <param name="j">= データオブジェクト</param>
    /// <returns>データ</returns>
    TypeWriterTimelineData* create_data(const json& j);

private:
    /// <summary>
    /// タイムラインデータを管理下に追加
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    /// <param name="data">= データ</param>
    void add(const string& name, TypeWriterTimelineData* data);

    /// <summary>
    /// タイムラインデータを検索
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    TypeWriterTimelineData* find(const std::string& name);

private:
    // タイムラインデータマップ
    std::unordered_map<std::string, TypeWriterTimelineData*> timelines_;
    // 現在再生中のタイムラインデータ
    TypeWriterTimelineData* current_{ nullptr };

};

#endif
