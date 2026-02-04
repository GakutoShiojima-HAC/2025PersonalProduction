#include "Engine/Core/Actor/ActorManager.h"
#include "Engine/Core/Actor/Actor.h"
#include <iterator>
#include <cassert>

#ifdef _DEBUG
#include <imgui/imgui.h>
#include "Engine/Utils/MyString.h"
#include "Engine/Utils/DebugMarker.h"
#endif

ActorManager::~ActorManager() {
	clear();
}

void ActorManager::add(Actor* actor) {
	actors_.push_back(actor);
}

void ActorManager::update(float delta_time, float scale_time) {
	for (const auto& actor : actors_) {
		actor->update(actor->is_enable_timescale() ? scale_time : delta_time);
	}

#ifdef _DEBUG
    ImGui::Begin("ActorDebugWindow");
    // アクターのコライダーを描画するかどうか
    {
        const std::string text = draw_collision_detection_ ? "Actorのコライダーを描画しない" : "Actorのコライダーを描画する";
        if (ImGui::Button(ToUTF8(text).c_str())) draw_collision_detection_ = !draw_collision_detection_;
    }
    // アクターのパラメータを描画するかどうか
    {
        const std::string text = draw_actor_parameter_ ? "Actorのパラメータを描画しない" : "Actorのパラメータを描画する";
        if (ImGui::Button(ToUTF8(text).c_str())) draw_actor_parameter_ = !draw_actor_parameter_;
    }
    ImGui::Separator();
    // デバッグ画面を表示するアクターの切り替え
    {
        ImGui::Text(ToUTF8("デバッグ対象のActorのIndex").c_str());
        std::string text = "max: " + std::to_string(actors_.size() - 1);
        ImGui::InputInt(text.c_str(), &debug_actor_index_, 1);
        debug_actor_index_ = std::max(0, std::min(debug_actor_index_, (int)actors_.size() - 1));
    }
    ImGui::Separator();
    // アクターのデバッグ画面を表示
    if (debug_actor_index_ > -1 && debug_actor_index_ < actors_.size() && !actors_.empty()) {
        auto it = actors_.begin();
        std::advance(it, debug_actor_index_);
        auto actor = *it;
        if (actor != nullptr) actor->debug_update(delta_time);
    }
    ImGui::End();
#endif
}

void ActorManager::late_update(float delta_time, float scale_time) {
	for (const auto& actor : actors_) {
		actor->late_update(actor->is_enable_timescale() ? scale_time : delta_time);
	}
}

void ActorManager::draw() const {
	for (const auto& actor : actors_) {
		actor->draw();

#ifdef _DEBUG
        // コライダーを描画
		if (draw_collision_detection_ && actor->is_collision()) actor->draw_collider();
        // パラメータを描画
        if (draw_actor_parameter_) {
            GSmatrix4 mat = actor->transform().localToWorldMatrix();
            GSvector3 position = mat.position();
            GSvector3 left = position + mat.left() * mat.scale().x * 0.5f;
            GSvector3 up = position + mat.up() * mat.scale().y * 0.5f;
            GSvector3 forward = position + mat.forward() * mat.scale().z * 0.5f;

            // ライティングを無効にする
            glDisable(GL_LIGHTING);
            // 線の太さを変更
            glLineWidth(4.0f);

            glColor3f(1.0f, 0.0f, 0.0f);	// 赤（左手方向）
            glBegin(GL_LINES);
            glVertex3f(position.x, position.y, position.z);
            glVertex3f(left.x, left.y, left.z);
            glEnd();
            glColor3f(0.0f, 1.0f, 0.0f);	// 緑（上方向）
            glBegin(GL_LINES);
            glVertex3f(position.x, position.y, position.z);
            glVertex3f(up.x, up.y, up.z);
            glEnd();
            glColor3f(0.0f, 0.0f, 1.0f);	// 青（前方向）
            glBegin(GL_LINES);
            glVertex3f(position.x, position.y, position.z);
            glVertex3f(forward.x, forward.y, forward.z);
            glEnd();

            // 白に戻す
            glColor3f(1.0f, 1.0f, 1.0f);
            // 線の太さを戻す
            glLineWidth(1.0f);
            // ライティングを有効にする
            glEnable(GL_LIGHTING);

            // 頭
            GSvector3 head = position + GSvector3{ 0.0f, actor->height(), 0.0f };
            MyLib::draw_sphere(head, 0.08f, GScolor{ 0.9f, 0.5f, 0.0f, 1.0f });
            // 足元
            GSvector3 foot = position + GSvector3{ 0.0f, -0.125f, 0.0f };
            MyLib::draw_sphere(foot, 0.08f, GScolor{ 0.9f, 0.5f, 0.0f, 1.0f });
            // 繋ぐ
            MyLib::draw_line(head, foot, GScolor{ 0.9f, 0.5f, 0.0f, 1.0f });
        }
#endif
	}
}

void ActorManager::draw_shadow() const {
    for (const auto& actor : actors_) {
        if (actor->enabled_cast_shadow()) actor->draw();
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
