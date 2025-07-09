#include "Scene/GameScene.h"

#include <gslib.h>	// tmp

void GameScene::start() {
	is_end_ = false;

}

void GameScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyTrigger(GKEY_L)) is_end_ = true;
}

void GameScene::draw() const {
	gsDrawText("game");
}

void GameScene::end() {

}

bool GameScene::is_end() const {
	return is_end_;
}

SceneTag GameScene::scene_tag() const {
	return SceneTag::Game;
}

SceneTag GameScene::next_scene_tag() const {
	return SceneTag::Menu;
}

bool GameScene::is_application_end() const {
	return false;
}

void GameScene::reception_message(const std::string& message, void* param) {
	// ‚È‚É‚àŽó‚¯Žæ‚ç‚È‚¢
}
