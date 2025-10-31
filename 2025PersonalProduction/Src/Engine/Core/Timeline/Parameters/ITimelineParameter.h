// -----------------------------------------------------------------------------------------
//  File          : ITimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : タイムラインパラメータのインターフェース
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef I_TIMELINE_PARAMETER_H_
#define I_TIMELINE_PARAMETER_H_

#include <string>
#include "Engine/Utils/MyJson.h"

typedef	unsigned int GSuint;

class World;

class ITimelineParameter {
public:
    ITimelineParameter(World* world) : world_{ world } {};

    virtual ~ITimelineParameter() = default;

public:
    virtual void update(float delta_time) = 0;

    virtual void clear() = 0;

public:
    /// <summary>
    /// パラメータの識別名を取得
    /// </summary>
    /// <returns>識別名</returns>
    virtual std::string name() const = 0;

    /// <summary>
    /// データを読み込む
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    /// <param name="j">= データオブジェクト</param>
    virtual void load(const std::string& name, const json& j) = 0;

    /// <summary>
    /// 指定した名前のタイムラインを再生
    /// </summary>
    /// <param name="name">= 名前</param>
    virtual void play(const std::string& name) = 0;

    /// <summary>
    /// 再生中のタイムラインを停止
    /// </summary>
    virtual void stop() = 0;

    /// <summary>
    /// タイムラインを再生中かどうか
    /// </summary>
    /// <returns>再生中なら真を返却</returns>
    virtual bool is_playing() const { return is_playing_; }

    /// <summary>
    /// ワールドを取得
    /// </summary>
    /// <returns>エディタ用 IWorldで返す</returns>
    virtual IWorld* get_world() { return world_; }

protected:
    // ワールド
    World* world_{ nullptr };
    // 再生中かどうか
    bool is_playing_{ false };
    // 再生タイマー
    float timer_{ 0.0f };
    // 現在のキーフレーム
    GSuint key_frame_{ 0 };

public:
    // コピー禁止
    ITimelineParameter(const ITimelineParameter& other) = delete;
    ITimelineParameter& operator = (const ITimelineParameter& other) = delete;

};

#endif
