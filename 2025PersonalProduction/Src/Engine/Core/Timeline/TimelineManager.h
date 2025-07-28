// -----------------------------------------------------------------------------------------
//  File          : TimelineManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/25
//  Updated       : 2025/07/25
//  Description   : タイムラインデータを再生、管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_MANAGER_H_
#define TIMELINE_MANAGER_H_

#include <vector>

using namespace std;

class ITimeline;
class CameraTimeline;

class TimelineManager {
public:
	TimelineManager() = default;

	~TimelineManager();

public:
	void update(float delta_time);

	void clear();

public:
	void add(CameraTimeline* timeline);

	CameraTimeline* camera_timeline();

private:
	vector<ITimeline*> timelines_;

	CameraTimeline* camera_timeline_{ nullptr };

public:
	// コピー禁止
	TimelineManager(const TimelineManager& other) = delete;
	TimelineManager& operator = (const TimelineManager& other) = delete;

};

#endif
