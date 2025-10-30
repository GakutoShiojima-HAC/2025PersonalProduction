#include "Engine/Core/NavMesh/NavMeshAgent.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include "GameConfig.h"

#ifdef _DEBUG
#include "Engine/Utils/DebugMarker.h"
#endif

// 再探索の時間
constexpr float RE_FIND_TIME{ 1.0f };

NavMeshAgent::NavMeshAgent(Actor* target, NavMeshSurface* navmesh) {
	target_ = target;
	navmesh_ = navmesh;
	path_index_ = 0;
    re_find_timer_ = 0.0f;
    offset_ratio_ = 0.25f;
}

NavMeshAgent::~NavMeshAgent() {

}

void NavMeshAgent::update_move(float delta_time, float move_speed, float rotate_angle) {
	if (target_ == nullptr) return;

	// たどり着いている
	if (path_index_ >= path_.size()) return;

    // 再探索を更新
    re_find_timer_ += delta_time / cFPS;
    if (re_find_timer_ >= RE_FIND_TIME) {
        re_find_timer_ = 0.0f;

        // 経路探索に成功したら(より良い道を見つけたら)
        if (navmesh_ != nullptr) {
            vector<GSvector3> path;
            navmesh_->find(target_->transform().position(), goal_position_, path, offset_ratio_);
            if (!path.empty()) {
                // 更新
                path_ = path;
                path_index_ = 0;
            }
        }
    }
    if (!found_path()) return;

	// 次の行き先を取得
	const GSvector3& target = path_[path_index_];

	// 行き先までの方向を計算
	GSvector3 direction = target - target_->transform().position();
	direction.y = 0.0f;
	const float direction_distance = direction.magnitude();
	direction.normalize();

	// 移動していたら向きの補間をする
	if (direction.magnitude() > 0.01f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				target_->transform().rotation(),
				GSquaternion::lookRotation(direction), rotate_angle * delta_time);

		target_->transform().rotation(rotation);
	}

	// 全体の移動量を計算
	GSvector3& velocity = target_->velocity();
	velocity = direction * move_speed * delta_time;

	// もし移動量が行き先までの長さを超えていたら
	if (velocity.magnitude() >= direction_distance) {
		// 移動量を補正
		velocity = velocity.normalize();
		velocity *= direction_distance;

		// 絶対値とする
		target_->transform().position(target);
		// 次の行き先へ
		++path_index_;
		return;
	}

	target_->transform().translate(velocity, GStransform::Space::World);
}

void NavMeshAgent::draw_path() const {
	if (!found_path()) return;

	for (int i = 0; i < (int)path_.size(); ++i) {
		GSvector3 pos = path_[i];
        pos.y += 0.2f;  // 少し上げないとNavMeshSurfaceと被る
		float radius = i == path_index_ ? 0.1 : 0.05;
		MyLib::draw_sphere(pos, radius, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });

		if (i >= (int)path_.size() - 1) continue;
		const GSvector3& end = path_[i + 1];
		MyLib::draw_line(pos, end, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

bool NavMeshAgent::find_path(const GSvector3& start, const GSvector3& end) {
	if (navmesh_ == nullptr) return false;

    reset_progress();
    goal_position_ = end;
	navmesh_->find(start, end, path_, offset_ratio_);
	return found_path();
}

bool NavMeshAgent::found_path() const {
	if (navmesh_ == nullptr) return false;
	return !path_.empty();
}

bool NavMeshAgent::end_move() const {
	return path_index_ >= path_.size();
}

const vector<GSvector3>& NavMeshAgent::path() {
	return path_;
}

void NavMeshAgent::reset_move() {
    reset_progress();
	if (found_path()) target_->transform().position(path_[0]);
}

float& NavMeshAgent::offset_ratio() {
    return offset_ratio_;
}

void NavMeshAgent::reset_progress() {
    path_index_ = 0;
    re_find_timer_ = 0.0f;
}
