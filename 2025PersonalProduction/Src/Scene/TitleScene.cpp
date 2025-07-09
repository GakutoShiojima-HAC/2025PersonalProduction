#include "Scene/TitleScene.h"

#include <gslib.h>	// tmp

void TitleScene::start() {
	is_end_ = false;

}

void TitleScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyTrigger(GKEY_L)) is_end_ = true;
}

void TitleScene::draw() const {
	gsDrawText("title");
}

void TitleScene::end() {

}

bool TitleScene::is_end() const {
	return is_end_;
}

SceneTag TitleScene::scene_tag() const {
	return SceneTag::Title;
}

SceneTag TitleScene::next_scene_tag() const {
	return SceneTag::Menu;
}

bool TitleScene::is_application_end() const {
	return false;
}

void TitleScene::reception_message(const std::string& message, void* param) {
	// ‚È‚É‚àŽó‚¯Žæ‚ç‚È‚¢
}
