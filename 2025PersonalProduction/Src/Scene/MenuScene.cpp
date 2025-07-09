#include "Scene/MenuScene.h"

#include <gslib.h>	// tmp

void MenuScene::start() {
	is_end_ = false;

}

void MenuScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyTrigger(GKEY_L)) is_end_ = true;
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
	return SceneTag::Game;
}

bool MenuScene::is_application_end() const {
	return false;
}

void MenuScene::reception_message(const std::string& message, void* param) {
	// ‚È‚É‚àŽó‚¯Žæ‚ç‚È‚¢
}
