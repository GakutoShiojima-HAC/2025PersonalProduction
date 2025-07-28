#include "Engine/Core/Timeline/TimelineManager.h"
#include "Engine/Core/Timeline/ITimeline.h"
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"

TimelineManager::~TimelineManager() {
	clear();
}

void TimelineManager::update(float delta_time) {
	for (auto& timeline : timelines_) {
		timeline->update(delta_time);
	}
}

void TimelineManager::clear() {
	for (auto timeline : timelines_) {
		timeline->clear();
		delete timeline;
		timeline = nullptr;
	}
	timelines_.clear();
	camera_timeline_ = nullptr;
}

void TimelineManager::add(CameraTimeline* timeline) {
	camera_timeline_ = timeline;
	timelines_.push_back(timeline);	// �������Ǘ��͔C����
}

CameraTimeline* TimelineManager::camera_timeline() {
	return camera_timeline_;
}

