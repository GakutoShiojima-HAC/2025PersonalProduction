#include "Scene/MenuScene.h"

#include <gslib.h>	// tmp

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

void MenuScene::start() {
	is_end_ = false;

}

void MenuScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyTrigger(GKEY_L)) is_end_ = true;

#ifdef _DEBUG
	ImGui::Begin("Scene Select Window");
	if (ImGui::Button("GameScene")) {
		is_end_ = true;
		next_scene_tag_ = SceneTag::Game;
	}
	if (ImGui::Button("TimelineEditorScene")) {
		is_end_ = true;
		next_scene_tag_ = SceneTag::TimelineEditor;
	}

	ImGui::End();
#endif
}

void MenuScene::draw() const {
	gsDrawText("menu");
}

void MenuScene::end() {

}

bool MenuScene::is_end() const {
	return is_end_;
}

SceneTag MenuScene::scene_tag() const {
	return SceneTag::Menu;
}

SceneTag MenuScene::next_scene_tag() const {
	return next_scene_tag_;
}

bool MenuScene::is_application_end() const {
	return false;
}

void MenuScene::reception_message(const std::string& message, void* param) {
	// ‚È‚É‚àŽó‚¯Žæ‚ç‚È‚¢
}
