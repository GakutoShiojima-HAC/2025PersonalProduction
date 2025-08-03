// -----------------------------------------------------------------------------------------
//  File          : TimelineEditorManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/29
//  Description   : タイムライン編集クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
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
	// コピー禁止
	TimelineEditorManager(const TimelineEditorManager& other) = delete;
	TimelineEditorManager& operator = (const TimelineEditorManager& other) = delete;

};

#endif
