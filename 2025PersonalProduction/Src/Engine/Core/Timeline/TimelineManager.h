// -----------------------------------------------------------------------------------------
//  File          : TimelineManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/25
//  Updated       : 2025/07/25
//  Description   : �^�C�����C���f�[�^���Đ��A�Ǘ�����N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
	// �R�s�[�֎~
	TimelineManager(const TimelineManager& other) = delete;
	TimelineManager& operator = (const TimelineManager& other) = delete;

};

#endif
