// -----------------------------------------------------------------------------------------
//  File          : CameraTimeline.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/25
//  Updated       : 2025/07/28
//  Description   : カメラ用タイムライン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_TIMELINE_H_
#define CAMERA_TIMELINE_H_

#include "Engine/Core/Timeline/ITimeline.h"
#include <gslib.h>

class Camera;

class CameraTimeline : public ITimeline {
public:
	CameraTimeline(World* world, const string& list_json);

	~CameraTimeline() = default;

public:
	void update(float delta_time) override;

	void clear() override;

	void play(const string& name) override;

	void stop() override;

	void end() override;

public:
	// キーフレーム
	struct CameraKeyFrame {
		CameraKeyFrame(
			float time,
			string target,
			GSvector3 position,
			GSvector3 lookat,
			float angle
		) :
			time{ time },
			target{ target },
			position{ position },
			lookat{ lookat },
			angle{ angle }
		{ }

		float time{ 0.0f };						// 時間(秒)
		string target{ "" };					// 基点とするアクター 存在しなければワールド原点
		GSvector3 position{ 0.0f, 0.0f, 0.0f };	// 基点からの座標
		GSvector3 lookat{ 0.0f, 0.0f, 0.0f };	// 注視点
		float angle{ 0.0f };					// 角度
	};

	// タイムラインデータ
	class CameraTimelineData {
	public:
		CameraTimelineData(const vector<CameraKeyFrame*>& timeline, float start_transition_time = 0.0f, float end_transition_time = 0.0f);

		~CameraTimelineData();

	public:
		void clear();

	public:
		/// <summary>
		/// タイムラインデータ
		/// </summary>
		vector<CameraKeyFrame*>& get();

	public:
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
		vector<CameraKeyFrame*> timeline_;
		// タイムライン用カメラに遷移するまでの時間
		float start_transition_time_{ 0.0f };
		// 元のカメラに遷移するまでの時間
		float end_transition_time_{ 0.0f };
	};

public:
	/// <summary>
	/// jsonを読み込みタイムラインデータで返却
	/// </summary>
	/// <param name="load_json_path">= json</param>
	/// <returns>= データ</returns>
	CameraTimelineData* load(const string& load_json_path);

	/// <summary>
	/// タイムラインデータを一度だけ再生 メモリ管理はしていません
	/// </summary>
	/// <param name="data">= データ</param>
	void play_data(CameraTimelineData* data);

private:
	/// <summary>
	/// タイムラインデータを管理下に追加
	/// </summary>
	/// <param name="name">= 識別名</param>
	/// <param name="data">= データ</param>
	void add(const string& name, CameraTimelineData* data);

	/// <summary>
	/// タイムラインデータを検索
	/// </summary>
	/// <param name="name">= 識別名</param>
	CameraTimelineData* find(const string& name);

private:
	// タイムラインデータマップ
	unordered_map<string, CameraTimelineData*> timelines_;
	// 現在再生中のタイムラインデータ
	CameraTimelineData* current_{ nullptr };
	// タイムライン用カメラ
	Camera* camera_{ nullptr };
	// 元のカメラ
	Camera* prev_camera_{ nullptr };
};

#endif
