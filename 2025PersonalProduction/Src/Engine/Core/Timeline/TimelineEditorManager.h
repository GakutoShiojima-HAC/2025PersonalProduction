// -----------------------------------------------------------------------------------------
//  File          : TimelineEditorManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/29
//  Description   : �^�C�����C���ҏW�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_EDITOR_MANAGER_H_
#define TIMELINE_EDITOR_MANAGER_H_

#include <vector>

using namespace std;

class TimelineEditor;

class TimelineEditorManager {
public:
	TimelineEditorManager() = default;

	~TimelineEditorManager();

public:
	void update(float delta_time);

	void clear();

	void add(TimelineEditor* editor);

private:
	vector<TimelineEditor*> editors_;

public:
	// �R�s�[�֎~
	TimelineEditorManager(const TimelineEditorManager& other) = delete;
	TimelineEditorManager& operator = (const TimelineEditorManager& other) = delete;

};

#endif
