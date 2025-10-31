#include "Engine/Core/Actor/ActorManager.h"
#include "Engine/Core/Actor/Actor.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

ActorManager::~ActorManager() {
	clear();
}

void ActorManager::add(Actor* actor) {
	actors_.push_back(actor);
}

void ActorManager::update(float delta_time, float scale_time) {
	for (const auto& actor : actors_) {
		actor->update(delta_time);
	}

#ifdef _DEBUG
	ImGui::Begin("Game Window");
	// 衝突判定を描画するかどうか
	if (ImGui::Button("draw collision detection")) draw_collision_detection_ = !draw_collision_detection_;
	ImGui::End();
#endif
}

void ActorManager::late_update(float delta_time, float scale_time) {
	for (const auto& actor : actors_) {
		actor->late_update(delta_time);
	}
}

void ActorManager::draw() const {
	for (const auto& actor : actors_) {
		actor->draw();

#ifdef _DEBUG
		if (!draw_collision_detection_) continue;
		if (actor->is_collision()) actor->draw_collider();
#endif
	}
}

void ActorManager::draw_tranparent() const {
	for (const auto& actor : actors_) {
		actor->draw_transparent();
	}
}

void ActorManager::draw_gui() const {
	for (const auto& actor : actors_) {
		actor->draw_gui();
	}
}

void ActorManager::collide() {
	// そんなアクター追加する予定ないのでシンプルに総当たり
	for (auto i = actors_.begin(); i != actors_.end(); ++i) {
		for (auto j = std::next(i); j != actors_.end(); ++j) {
			(*i)->collide(**j);
		}
	}
}

void ActorManager::remove() {
	for (auto i = actors_.begin(); i != actors_.end(); ) {
		if ((*i)->is_dead()) {
			if ((*i)->is_clear()) delete* i;
			i = actors_.erase(i);
		}
		else {
			++i;
		}
	}
}

std::vector<Actor*> ActorManager::get_all() const {
    return std::vector<Actor*>(actors_.begin(), actors_.end());
}

Actor* ActorManager::find(const string& name) const {
	for (const auto& actor : actors_) {
		if (actor->name() == name) {
			return actor;
		}
	}
	return nullptr;
}

std::vector<Actor*> ActorManager::find_with_tag(const ActorTag tag) const {
    std::vector<Actor*> actors;
	for (const auto& actor : actors_) {
		if (actor->tag() == tag) {
			actors.push_back(actor);
		}
	}
	return actors;
}

int ActorManager::count() const {
	return actors_.size();
}

int ActorManager::count_with_tag(const ActorTag tag) const {
	int count{ 0 };
	for (const auto& actor : actors_) {
		if (actor->tag() == tag) {
			++count;
		}
	}

	return count;
}

void ActorManager::clear() {
	for (auto actor : actors_) {
		if (actor->is_clear()) delete actor;
	}
	actors_.clear();
}
