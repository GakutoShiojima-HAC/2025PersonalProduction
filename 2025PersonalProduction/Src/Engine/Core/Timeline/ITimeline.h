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

protected:
	/// <summary>
	/// タイムライン再生を終了
	/// </summary>
	virtual void end() = 0;

	/// <summary>
	/// タイムラインデータを読み込む
	/// </summary>
	/// <param name="name">= 管理名</param>
	/// <param name="data_json_path">= データのパス</param>
	virtual void load(const string& name, const string& data_json_path) = 0;

	/// <summary>
	/// indexがvectorの中に存在するかどうか
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="vec"></param>
	/// <param name="index"></param>
	/// <returns>存在するなら真</returns>
	template <typename T>
	inline T* is_valid_index(const vector<T*>& vec, GSint index) {
		if (vec.empty()) return nullptr;
		return index >= 0 && (size_t)index < vec.size() ? vec[index] : nullptr;
	}

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
