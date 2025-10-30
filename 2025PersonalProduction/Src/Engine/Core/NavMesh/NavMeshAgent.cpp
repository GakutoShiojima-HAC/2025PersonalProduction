#include "Engine/Core/NavMesh/NavMeshAgent.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include "GameConfig.h"

#ifdef _DEBUG
#include "Engine/Utils/DebugMarker.h"
#endif

// �ĒT���̎���
constexpr float RE_FIND_TIME{ 1.0f };

NavMeshAgent::NavMeshAgent(Actor* target, NavMeshSurface* navmesh) {
	target_ = target;
	navmesh_ = navmesh;
	path_index_ = 0;
    re_find_timer_ = 0.0f;
}

NavMeshAgent::~NavMeshAgent() {

}

void NavMeshAgent::update_move(float delta_time, float move_speed, float rotate_angle) {
	if (target_ == nullptr) return;

	// ���ǂ蒅���Ă���
	if (path_index_ >= path_.size()) return;

    // �ĒT�����X�V
    re_find_timer_ += delta_time / cFPS;
    if (re_find_timer_ >= RE_FIND_TIME) {
        re_find_timer_ = 0.0f;
        find_path(target_->transform().position(), goal_position_);
    }
    if (!found_path()) return;

	// ���̍s������擾
	const GSvector3& target = path_[path_index_];

	// �s����܂ł̕������v�Z
	GSvector3 direction = target - target_->transform().position();
	direction.y = 0.0f;
	const float direction_distance = direction.magnitude();
	direction.normalize();

	// �ړ����Ă���������̕�Ԃ�����
	if (direction.magnitude() > 0.01f) {
		// �����̕��
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				target_->transform().rotation(),
				GSquaternion::lookRotation(direction), rotate_angle * delta_time);

		target_->transform().rotation(rotation);
	}

	// �S�̂̈ړ��ʂ��v�Z
	GSvector3& velocity = target_->velocity();
	velocity = direction * move_speed * delta_time;

	// �����ړ��ʂ��s����܂ł̒����𒴂��Ă�����
	if (velocity.magnitude() >= direction_distance) {
		// �ړ��ʂ�␳
		velocity = velocity.normalize();
		velocity *= direction_distance;

		// ��Βl�Ƃ���
		target_->transform().position(target);
		// ���̍s�����
		++path_index_;
		return;
	}

	target_->transform().translate(velocity, GStransform::Space::World);
}

void NavMeshAgent::draw_path() const {
	if (!found_path()) return;

	for (int i = 0; i < (int)path_.size(); ++i) {
		const GSvector3& pos = path_[i];
		float radius = i == path_index_ ? 0.1 : 0.05;
		MyLib::draw_sphere(pos, radius, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });

		if (i >= (int)path_.size() - 1) continue;
		const GSvector3& end = path_[i + 1];
		MyLib::draw_line(pos, end, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

bool NavMeshAgent::find_path(const GSvector3& start, const GSvector3& end, float offset_ratio) {
	if (navmesh_ == nullptr) return false;

	path_index_ = 0;
    re_find_timer_ = 0.0f;
    goal_position_ = end;
	navmesh_->find(start, end, path_, offset_ratio);
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
    re_find_timer_ = 0.0f;
	if (found_path()) target_->transform().position(path_[0]);
}
