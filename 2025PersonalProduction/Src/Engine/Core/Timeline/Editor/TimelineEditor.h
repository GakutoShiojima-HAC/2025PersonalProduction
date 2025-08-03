// -----------------------------------------------------------------------------------------
//  File          : ITimelineEditor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : タイムラインエディタのインターフェース
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_EDITOR_H_
#define TIMELINE_EDITOR_H_

#include <string>
#include <vector>
#include <algorithm>
#include "GameConfig.h"
#include <imgui/imgui.h>
#include "Lib/imgui_stdlib.h"
#include "Engine/Core/World/World.h"

using namespace std;

typedef	unsigned int GSuint;

// 動かしていない
const int NOMOVE{ -1 };

class TimelineEditor {
public:
	TimelineEditor() = default;

	virtual ~TimelineEditor() = default;

public:
	virtual void update(float delta_time) = 0;

	virtual void clear() = 0;

protected:
	/// <summary>
	/// 編集中のキーフレーム値を上下させる
	/// </summary>
	/// <param name="value">= 移動量</param>
	/// <param name="vec_size">= キーフレームの量</param>
	void set_keyframe(int value, int vec_size);

	/// <summary>
	/// キーフレームの移動ボタンを描画する
	/// </summary>
	/// <param name="max_size">= 管理しているキーフレームの量</param>
	void draw_move_keyframe(int max_size);

protected:
	/// <summary>
	/// タイムラインエディタのメイン画面を描画する
	/// </summary>
	/// <param name="timeline_is_empty">= タイムラインデータが存在するか</param>
	/// <param name="file_path">= タイムラインデータの保存先</param>
	virtual void draw_main_gui(bool timeline_is_empty, const string& file_path);

	/// <summary>
	/// タイムラインデータの読み込みボタンを描画する
	/// </summary>
	/// <param name="file_path">= タイムラインデータの保存先</param>
	virtual void draw_load(const string& file_path);
	
	/// <summary>
	/// タイムラインデータの保存ボタンを描画する
	/// </summary>
	/// <param name="timeline_is_empty">= タイムラインデータが存在するか</param>
	/// <param name="file_path">= タイムラインデータの保存先</param>
	virtual void draw_save(bool timeline_is_empty, const string& file_path);
	
	/// <summary>
	/// タイムラインデータの再生ボタンを描画する
	/// </summary>
	virtual void draw_play();

	/// <summary>
	/// タイムラインデータのリセットボタンを描画する
	/// </summary>
	virtual void draw_reset();

	/// <summary>
	/// タイムラインを描画する
	/// </summary>
	/// <param name="timeline_is_empty">= タイムラインデータが存在するか</param>
	/// <param name="max_size">= 管理しているキーフレームの量</param>
	virtual void draw_timeline(bool timeline_is_empty, int max_size);

protected:
	/// <summary>
	/// キーフレーム追加画面を描画する
	/// </summary>
	virtual void draw_add_keyframe() = 0;

	/// <summary>
	/// 現在のキーフレーム編集画面を描画する
	/// </summary>
	virtual void draw_current_keyframe() = 0;

	/// <summary>
	/// タイムラインを再生する
	/// </summary>
	virtual void play() = 0;

	/// <summary>
	/// タイムラインデータを保存する
	/// </summary>
	virtual void save() = 0;

	/// <summary>
	/// 既存のタイムラインデータを読み込む
	/// </summary>
	virtual void load() = 0;

	/// <summary>
	/// 編集中のタイムラインデータをリセットする
	/// </summary>
	virtual void reset() = 0;

	/// <summary>
	/// タイムラインデータをキーフレームの時間順にソートする
	/// </summary>
	virtual void sort_timeline() = 0;

	/// <summary>
	/// 指定したキーフレームを削除する
	/// </summary>
	/// <param name="index">= キーフレーム番号</param>
	virtual void remove_key_frame(GSuint index) = 0;

	/// <summary>
	/// キーフレームの時間を取得する
	/// </summary>
	/// <param name="index">= キーフレーム番号</param>
	/// <returns>時間の参照</returns>
	virtual float& get_time(int index) = 0;

protected:
	// ワールド
	World* world_{ nullptr };
	// 編集中のキーフレーム
	GSuint current_edit_keyframe_{ 0 };
	// マウスで移動中のキーフレーム
	int current_move_keyframe_{ NOMOVE };
	// キーフレーム追加時間
	float time_{ 0.0f };
	// タイムラインの描画開始時間
	float timeline_start_time_{ 0.0f };
	// 保存先のファイル名
	string save_filename_{ "" };
	// ロード元のファイル名
	string load_filename_{ "" };

protected:
	// タイムライン描画の幅
	const float TIMELINE_MAX_TIME{ 5.0f };
	// タイムライン描画の高さ
	const float TIMELINE_HEIGHT{ 40.0f };

public:
	// コピー禁止
	TimelineEditor(const TimelineEditor& other) = delete;
	TimelineEditor& operator = (const TimelineEditor& other) = delete;

};

#endif
