#include "Engine/Core/Scene/SceneManager.h"
#include "Engine/Core/Scene/IScene.h"
#include "Engine/Core/Scene/NullScene.h"

static NullScene null_scene;

SceneManager::~SceneManager() {
	clear();
}

SceneManager& SceneManager::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static SceneManager self;
	return self;
}

void SceneManager::init() {
	current_scene_ = &null_scene;
}

void SceneManager::update(float delta_time) {
	current_scene_->update(delta_time);

	if (current_scene_->is_end()) {
		change(current_scene_->next_scene_tag());
	}
}

void SceneManager::draw() {
	current_scene_->draw();
}

void SceneManager::end() {
	current_scene_->end();

	current_scene_ = &null_scene;
}

void SceneManager::clear() {
	// シーンのインスタンスを消去
	for (auto& scene : scenes_) {
		delete scene;
	}
	scenes_.clear();
}

void SceneManager::add(IScene* scene) {
	scenes_.push_back(scene);
}

void SceneManager::change(SceneTag tag) {
	end();

	IScene* scene = find(tag);
	if (scene != nullptr) {
		current_scene_ = scene;
		scene->start();
	}
}

vector<IScene*> SceneManager::get_scenes() const {
	return scenes_;
}

int SceneManager::count() const {
	return (int)scenes_.size();
}

IScene* SceneManager::get_current_scene() const {
	return current_scene_;
}

SceneTag SceneManager::get_current_scene_tag() const {
	return current_scene_->scene_tag();
}

IScene* SceneManager::find(SceneTag tag) const {
	for (auto& scene : scenes_) {
		if (scene->scene_tag() == tag) {
			return scene;
		}
	}
	return nullptr;
}

bool SceneManager::is_application_end() const {
	return current_scene_->is_application_end();
}

void SceneManager::send_message(SceneTag tag, const std::string& message, std::any& param) {
	IScene* scene = find(tag);
	if (scene != nullptr) scene->reception_message(message, param);
}

bool SceneManager::load_scene(SceneTag tag) {
    IScene* scene = find(tag);
    if (scene != nullptr) {
        scene->load();
        return true;
    }
    return false;
}

bool SceneManager::is_load_end(SceneTag tag) const {
    IScene* scene = find(tag);
    if (scene != nullptr) return scene->is_load_end();
    return false;
}

float SceneManager::load_progress(SceneTag tag) const {
    IScene* scene = find(tag);
    if (scene != nullptr) return scene->load_progress();
    return -1.0f;
}
