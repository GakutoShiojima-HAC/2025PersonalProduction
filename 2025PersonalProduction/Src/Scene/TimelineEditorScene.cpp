#include "Scene/TimelineEditorScene.h"
#include <gslib.h>
#include "Engine/Core/Timeline/Editor/CameraTimelineEditor.h"

TimelineEditorScene::TimelineEditorScene() {
    next_scene_tag_ = SceneTag::Menu;
}

void TimelineEditorScene::start() {
    is_end_ = false;

    game_start();

    editor_.add(new CameraTimelineEditor{ &world_ });
}

void TimelineEditorScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;

	world_.update(delta_time);
	editor_.update(delta_time);
}

void TimelineEditorScene::draw() const {
	world_.draw();

	gsDrawText("timeline editor");
}

void TimelineEditorScene::end() {
    game_end();

    // タイムラインエディタクリア
	editor_.clear();
}

SceneTag TimelineEditorScene::scene_tag() const {
	return SceneTag::TimelineEditor;
}

bool TimelineEditorScene::is_application_end() const {
	return false;
}

void TimelineEditorScene::reception_message(const std::string& message, std::any& param) {
	// なにも受け取らない
}
