#include "Scene/TimelineEditorScene.h"
#include <gslib.h>

#include "State/Scene/SceneState.h"

TimelineEditorScene::TimelineEditorScene() {
    next_scene_tag_ = SceneTag::Menu;
}

void TimelineEditorScene::start() {
    is_end_ = false;

    game_start();

    //editor_.add(new CameraTimelineEditor{ &world_ });

    change_state((GSuint)SceneStateType::Original);
}

void TimelineEditorScene::end() {
    game_end();
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

void TimelineEditorScene::original_update(float delta_time) {
    // tmp scene end
    if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;

    world_.update(delta_time);
}

void TimelineEditorScene::original_draw() const {
    world_.draw();

    gsDrawText("timeline editor");
}
