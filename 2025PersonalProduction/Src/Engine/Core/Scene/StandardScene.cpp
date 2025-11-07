#include "Engine/Core/Scene/StandardScene.h"

void StandardScene::change_state(const GSuint state_num) {
    // 状態の変更
    state_.change_state(state_num);
}

bool StandardScene::is_end() const {
    return is_end_;
}

SceneTag StandardScene::scene_tag() const {
    return SceneTag::Standard;
}

SceneTag StandardScene::next_scene_tag() const {
    return SceneTag::Loading;
}

bool StandardScene::is_application_end() const {
    return is_app_end_;
}

bool StandardScene::is_load_end() const {
    return is_load_end_;
}

float StandardScene::load_progress() const {
    return load_progress_;;
}

void StandardScene::scene_end_request() {
    is_end_ = true;
}

void StandardScene::set_next_scene(SceneTag tag) {
    next_scene_tag_ = tag;
}

void StandardScene::app_end_request() {
    is_app_end_ = true;
}
