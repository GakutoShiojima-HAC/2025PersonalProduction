// -----------------------------------------------------------------------------------------
//  File          : ITimeline.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/25
//  Updated       : 2025/07/25
//  Description   : タイムラインデータのインターフェース
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef I_TIMELINE_H_
#define I_TIMELINE_H_

#include <unordered_map>	// keyframe管理用
#include <string>
#include <vector>
#include "GameConfig.h"

using namespace std;

typedef	unsigned int GSuint;

class World;

class ITimeline {
public:
	ITimeline() = default;

	virtual ~ITimeline() = default;

public:
	virtual void update(float delta_time) = 0;

	virtual void clear() = 0;

	/// <summary>
	/// タイムライン再生を開始
	/// </summary>
	/// <param name="name">= 管理名</param>
	virtual void play(const string& name) = 0;

	/// <summary>
	/// タイムライン再生を停止
	/// </summary>
	virtual void stop() = 0;

	/// <summary>
	/// タイムラインを再生しているかどうかを返却
	/// </summary>
	/// <returns>再生中なら真を返却</returns>
	virtual bool is_playing() const { return is_playing_; }

protected:
	/// <summary>
	/// タイムライン再生を終了
	/// </summary>
	virtual void end() = 0;

protected:
	// ワールド
	World* world_{ nullptr };
	// 現在再生しているキーフレーム番号
	GSint play_frame_{ 0 };
	// 再生開始からのタイマー
	float timer_{ 0.0f };
	// 再生中か
	bool is_playing_{ false };

public:
	// コピー禁止
	ITimeline(const ITimeline& other) = delete;
	ITimeline& operator = (const ITimeline& other) = delete;

};

#endif
