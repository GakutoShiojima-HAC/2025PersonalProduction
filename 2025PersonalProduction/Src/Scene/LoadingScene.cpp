#include "Scene/LoadingScene.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include <gslib.h>
#include "Engine/Core/Tween/Tween.h"

void LoadingScene::start() {
    is_end_ = false;

    // TODO loading image asset load

    // 次のシーンのロード処理を非同期で呼び出す
    gslib::Game::run_thread([=] {
        scene_manager_.load_scene(next_scene_tag_);
    });
}

void LoadingScene::update(float delta_time) {
    Tween::update(delta_time);
    // 次のシーンのロード処理が終了したらシーンを終了する
    if (scene_manager_.is_load_end(next_scene_tag_)) is_end_ = true;
}

void LoadingScene::draw() const {
    // 進捗率仮描画
    std::string text = "progress: " + to_string(scene_manager_.load_progress(next_scene_tag_) * 100.0f);
    Canvas::draw_text(text, GSvector2{ 0.0f, 0.0f }, 50.0f);
}

void LoadingScene::end() {
    Tween::clear();

}

bool LoadingScene::is_end() const {
    return is_end_;
}

SceneTag LoadingScene::scene_tag() const {
    return SceneTag::Loading;
}

SceneTag LoadingScene::next_scene_tag() const {
    return next_scene_tag_;
}

bool LoadingScene::is_application_end() const {
    return false;
}

void LoadingScene::reception_message(const std::string& message, std::any& param) {
    if (message == "NextSceneTag" && param.type() == typeid(SceneTag)) {
        next_scene_tag_ = std::any_cast<SceneTag>(param);
    }
}

bool LoadingScene::is_load_end() const {
    return true;
}

float LoadingScene::load_progress() const {
    return 1.0f;
}
