// -----------------------------------------------------------------------------------------
//  File          : Timeline.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : タイムラインを再生、パラメータを管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_H_
#define TIMELINE_H_

#include <string>
#include <vector>
#include "TimelineEditor.h"

class World;
class ITimelineParameter;

class Timeline {
public:
    Timeline() = default;

    ~Timeline();

public:
    void update(float delta_time, float scale_time);

    void clear();

public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="world">= ワールド</param>
    /// <param name="enable_editor">= エディタ機能を有効にするかどうか</param>
    void init(World* world, bool enable_editor);

    /// <summary>
    /// タイムラインデータを読み込む
    /// </summary>
    /// <param name="json_file"></param>
    void load(const std::string& json_file);

    /// <summary>
    /// 再生
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    void play(const std::string& name);

    /// <summary>
    /// 停止
    /// </summary>
    void stop();

    /// <summary>
    /// 再生中かどうか
    /// </summary>
    /// <returns></returns>
    bool is_playing() const;

private:
    /// <summary>
    /// タイムラインパラメータを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つからなかったらnullptr</returns>
    ITimelineParameter* find(const std::string& name);

private:
    std::vector<ITimelineParameter*> timelines_;

    TimelineEditor editor_;

public:
    // コピー禁止
    Timeline(const Timeline& other) = delete;
    Timeline& operator = (const Timeline& other) = delete;

};

#endif

