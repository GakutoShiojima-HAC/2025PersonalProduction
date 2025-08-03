#include "Engine/Core/Timeline/TimelineEditorManager.h"
#include "Engine/Core/Timeline/Editor/TimelineEditor.h"

TimelineEditorManager::~TimelineEditorManager() {
	clear();
}

void TimelineEditorManager::update(float delta_time) {
	for (auto& editor : editors_) {
		editor->update(delta_time);
	}
}

void TimelineEditorManager::clear() {
	for (auto& editor : editors_) {
		editor->clear();
		delete editor;
		editor = nullptr;
	}
	editors_.clear();
}

void TimelineEditorManager::add(TimelineEditor* editor) {
	editors_.push_back(editor);
}
