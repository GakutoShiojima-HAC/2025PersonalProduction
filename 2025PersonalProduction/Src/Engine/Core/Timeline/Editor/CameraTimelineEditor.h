// -----------------------------------------------------------------------------------------
//  File          : CameraTimelineEditor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : カメラ用タイムラインのエディタ
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_TIMELINE_EDITOR_H_
#define CAMERA_TIMELINE_EDITOR_H_

#include "Engine/Core/Timeline/Editor/TimelineEditor.h"
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"

using CameraKeyFrame = CameraTimeline::CameraKeyFrame;
using CameraTimelineData = CameraTimeline::CameraTimelineData;

class World;

class CameraTimelineEditor : public TimelineEditor {
public:
	CameraTimelineEditor(World* world);

	~CameraTimelineEditor();

public:
	void update(float delta_time) override;

	void clear() override;

private:
	void draw_add_keyframe() override;

	void draw_current_keyframe() override;

	void play() override;

	void save() override;

	void load() override;

	void reset() override;

	void sort_timeline() override;

	void remove_key_frame(GSuint index) override;

	float& get_time(int index) override;

private:
	/// <summary>
	/// キーフレームの追加
	/// </summary>
	/// <param name="time">= タイムライン上の時間</param>
	/// <param name="target">= 原点とするターゲット名</param>
	/// <param name="position">= 原点からのカメラ座標</param>
	/// <param name="lookat">= 原点からの注視点</param>
	/// <param name="angle">= カメラの傾き</param>
	void add_key_frame(float time, const string& target, const GSvector3& position, const GSvector3& lookat, float angle);

	/// <summary>
	/// カメラの傾き角度を取得
	/// </summary>
	/// <param name="rotation">= 回転</param>
	/// <returns>= 角度(度)</returns>
	float get_tilt_angle(const GSquaternion& rotation) const;

private:	 
	CameraTimelineData* timeline_{ nullptr };

	// 原点にするターゲット
	string target_{ "" };

};

#endif
