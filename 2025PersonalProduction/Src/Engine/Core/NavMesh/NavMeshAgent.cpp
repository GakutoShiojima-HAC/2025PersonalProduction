#include "Engine/Core/NavMesh/NavMeshAgent.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"

#ifdef _DEBUG
#include "Engine/Utils/DebugMarker.h"
#endif

NavMeshAgent::NavMeshAgent(Actor* target, NavMeshSurface* navmesh) {
	target_ = target;
	navmesh_ = navmesh;
	path_index_ = 0;
}

NavMeshAgent::~NavMeshAgent() {

}

void NavMeshAgent::update_move(float delta_time, float move_speed, float rotate_angle) {
	if (!found_path() || target_ == nullptr) return;

	// たどり着いている
	if (path_index_ >= path_.size()) return;

	// 次の行き先を取得
	const GSvector3& target = path_[path_index_];

	// 行き先までの方向を計算
	GSvector3 direction = target - target_->transform().position();
	direction.y = 0.0f;
	const float direction_distance = direction.magnitude();
	direction.normalize();

	// 移動していたら向きの補間をする
	if (direction.magnitude() > 0.01f) {
		float angle_ = atan2(direction.x, direction.z);

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

	for (int i = 0; i < path_.size(); ++i) {
		const GSvector3& pos = path_[i];
		float radius = i == path_index_ ? 0.075 : 0.05;
		MyLib::draw_sphere(pos, radius, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });

		if (i >= path_.size() - 1) continue;
		const GSvector3& end = path_[i + 1];
		MyLib::draw_line(pos, end, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void NavMeshAgent::draw_line_path() const {
#ifdef _DEBUG
	if (navmesh_ == nullptr || line_path_.empty()) return;

	for (int i = 0; i < line_path_.size(); ++i) {
		const GSvector3& pos = line_path_[i];
		float radius = i == path_index_ + 1 ? 0.075 : 0.05;
		MyLib::draw_sphere(pos, radius, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });

		if (i >= line_path_.size() - 1) continue;
		const GSvector3& end = line_path_[i + 1];
		MyLib::draw_line(pos, end, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
#endif
}

bool NavMeshAgent::find_path(const GSvector3& start, const GSvector3& end) {
	if (navmesh_ == nullptr) return false;

	vector<int> path = navmesh_->find_path(start, end);

	path_index_ = 0;
	path_ = navmesh_->create_smooth_path(path, start, end);

#ifdef _DEBUG
	line_path_ = navmesh_->create_line_path(path);
#endif

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
	path_index_ = 0;
	if (found_path()) target_->transform().position(path_[0]);
}
