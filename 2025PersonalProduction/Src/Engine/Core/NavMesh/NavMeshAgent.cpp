#include "Engine/Core/NavMesh/NavMeshAgent.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include "Engine/Core/Actor/Actor.h"
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
    offset_ratio_ = 0.25f;
}

NavMeshAgent::~NavMeshAgent() {
    goal_actor_ = nullptr;
}

void NavMeshAgent::update_move(float delta_time, float move_speed, float rotate_angle) {
    // �ړ�������Ώۂ����Ȃ�
	if (target_ == nullptr) return;
	// ���ǂ蒅���Ă���
	if (goal_actor_ == nullptr && is_end_move()) return;

    // �ĒT�����X�V
    {
        re_find_timer_ += delta_time / cFPS;
        if (re_find_timer_ >= RE_FIND_TIME) {
            re_find_timer_ = 0.0f;

            // �ĒT���h�~
            if (goal_actor_ != nullptr && goal_actor_->is_dead()) {
                end();
                return;
            }
            // �S�[���n�_
            const GSvector3 goal = goal_actor_ == nullptr ? goal_position_ : goal_actor_->transform().position();

            // �o�H�T���ɐ���������(���ǂ�������������)
            if (navmesh_ != nullptr) {
                vector<GSvector3> path;
                navmesh_->find(target_->transform().position(), goal, path, offset_ratio_);
                if (!path.empty()) {
                    // �X�V
                    path_ = path;
                    path_index_ = 0;
                }
            }
        }
    }

    // �o�H���Ȃ�
    if (!found_path() || is_end_move()) return;

    // �ړ�
    {
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
        // ������ꏊ������Ă���͂��Ȃ̂ŏՓ˔���͍s��Ȃ�
        target_->transform().translate(velocity, GStransform::Space::World);
    }

    // �ړ����I�����Ă�����I��
    if (goal_actor_ == nullptr && is_end_move()) end();
}

void NavMeshAgent::draw_path() const {
	if (!found_path()) return;

	for (int i = 0; i < (int)path_.size(); ++i) {
		GSvector3 pos = path_[i];
        pos.y += 0.2f;  // �����グ�Ȃ���NavMeshSurface�Ɣ��
		float radius = i == path_index_ ? 0.1 : 0.05;
		MyLib::draw_sphere(pos, radius, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });

		if (i >= (int)path_.size() - 1) continue;
		const GSvector3& end = path_[i + 1];
		MyLib::draw_line(pos, end, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

bool NavMeshAgent::find_path(const GSvector3& end) {
	if (navmesh_ == nullptr || target_ == nullptr) return false;

    path_index_ = 0;
    re_find_timer_ = 0.0f;
    goal_actor_ = nullptr;
    goal_position_ = end;
	navmesh_->find(target_->transform().position(), end, path_, offset_ratio_);
	return found_path();
}

bool NavMeshAgent::find_path(Actor* goal) {
    if (navmesh_ == nullptr || target_ == nullptr || goal == nullptr) return false;

    path_index_ = 0;
    re_find_timer_ = 0.0f;
    goal_actor_ = goal;
    goal_position_ = goal_actor_->transform().position();
    navmesh_->find(target_->transform().position(), goal_actor_->transform().position(), path_, offset_ratio_);
    return found_path();
}

bool NavMeshAgent::found_path() const {
	if (navmesh_ == nullptr) return false;
	return !path_.empty();
}

bool NavMeshAgent::is_end_move() const {
	return path_index_ >= path_.size();
}

const vector<GSvector3>& NavMeshAgent::path() {
	return path_;
}

void NavMeshAgent::end() {
    path_.clear();
    path_index_ = 0;
    re_find_timer_ = 0.0f;
    goal_actor_ = nullptr;
}

float& NavMeshAgent::offset_ratio() {
    return offset_ratio_;
}
