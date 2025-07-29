#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Actor/Actor.h"
#include "Lib/json.hpp"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Utils/Check.h"

using json = nlohmann::json;

CameraTimeline::CameraTimeline(World* world, const string& list_json) {
	world_ = world;

	ifstream file(list_json);
	if (!file.is_open()) return;
	json j;
	file >> j;

	// ���ʖ��ƃ^�C�����C���f�[�^�t�@�C���p�X��n���ēǂݍ���
	for (auto it = j.begin(); it != j.end(); ++it) {
		add(it.key(), load(it.value()));
	}
}

void CameraTimeline::update(float delta_time) {
	if (!is_playing_) {
		return;
	}
	if (current_ == nullptr || camera_ == nullptr) {
		end();
		return;
	}

	// play�̌��update���Ă΂��s�����ɏ���
	CameraKeyFrame* current{ nullptr };
	MyLib::is_valid_index(current_->get(), play_frame_, &current);
	CameraKeyFrame* next{ nullptr };
	MyLib::is_valid_index(current_->get(), play_frame_ + 1, &next);

	// ���̃L�[�t���[�����Ȃ��Ȃ�����I��
	if (current == nullptr || next == nullptr) {
		end();
		return;
	}
	// �ŏ��̃L�[�t���[�����Đ������ŏ��̃L�[�t���[���J�n���Ԃ��n�܂��Ă��Ȃ�������^�C�}�[���X�V���ďI��
	if (play_frame_ == 0 && current->time > timer_) {
		timer_ += delta_time / cFPS;
		if (current->time <= timer_) {
			world_->camera_transition(camera_, current_->start_transition_time());
		}
		return;
	}
	// �J�n�t���[���Ȃ�J������؂�ւ���
	if (play_frame_ == 0 && current->time >= timer_) {
		world_->camera_transition(camera_, current_->start_transition_time());
	}

	// �J�����p�����[�^���X�V
	const float progress = CLAMP((timer_ - current->time) / (next->time - current->time), 0.0f, 1.0f);
	Actor* current_target = world_->find_actor(current->target);
	Actor* next_target = world_->find_actor(next->target);

	auto get_position = [](Actor* target, const GSvector3& local_position) {
		if (target == nullptr) return local_position;
		// �w�肳�ꂽTranslate, Rotation, Scale�̍s����쐬����
		GSmatrix4 local_matrix = GSmatrix4::TRS(local_position, GSquaternion::euler(GSvector3{ 0.0f, 0.0f, 0.0f }), GSvector3{ 1.0f, 1.0f, 1.0f });
		return (local_matrix * target->transform().localToWorldMatrix()).position();
	};

	// ���W
	const GSvector3 current_position = get_position(current_target, current->position);
	const GSvector3 next_position = get_position(next_target, next->position);
	const GSvector3 position = GSvector3::lerp(current_position, next_position, progress);
	camera_->transform().position(position);
	// �����_
	const GSvector3 current_lookat = get_position(current_target, current->lookat);
	const GSvector3 next_lookat = get_position(next_target, next->lookat);
	const GSvector3 lookat = GSvector3::lerp(current_lookat, next_lookat, progress);
	camera_->transform().lookAt(lookat, GSvector3::up());
	// �p�x
	auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
	camera_->transform().rotation(camera_->transform().rotation() * GSquaternion::angleAxis(lerp(current->angle, next->angle, progress), GSvector3::forward()));

	// �Đ��L�[�t���[�����X�V
	timer_ += delta_time / cFPS;
	if (progress >= 1.0f) {
		++play_frame_;
	}
}

void CameraTimeline::clear() {
	end();
	for (auto i : timelines_) {
		i.second->clear();
		delete i.second;
		i.second = nullptr;
	}
	timelines_.clear();
}

void CameraTimeline::play(const string& name) {
	// �Đ����Ȃ��x�I��
	if (is_playing_) end();
	// �Đ�����^�C�����C���f�[�^���擾
	current_ = find(name);
	// ������Ȃ���΍Đ������I��
	if (current_ == nullptr) {
		end();
		return;
	}

	// ������
	is_playing_ = true;
	play_frame_ = 0;
	timer_ = 0.0f;
	prev_camera_ = world_->get_camera();
	camera_ = world_->find_camera(CameraTag::Timeline);
}

void CameraTimeline::stop() {
	is_playing_ = false;
}

void CameraTimeline::end() {
	if (prev_camera_ != nullptr) world_->camera_transition(prev_camera_, current_ != nullptr ? current_->end_transition_time() : 0.0f);
	stop();
	current_ = nullptr;
	camera_ = nullptr;
	prev_camera_ = nullptr;
}

CameraTimeline::CameraTimelineData* CameraTimeline::load(const string& load_json_path) {
	ifstream file(load_json_path);
	if (!file.is_open()) return;
	json j;
	file >> j;

	// �G���[�̏o�Ȃ��^�C���擾
	auto get_time = [](const json& json, const string& key) {
		return json.contains(key) && json[key].is_number() ? json[key].get<float>() : 0.0f;
	};

	// �^�C�����C���p�J�����ɑJ�ڂ���܂ł̎���
	const float start = get_time(j, "start");
	// ���̃J�����ɑJ�ڂ���܂ł̎���
	const float end = get_time(j, "end");
	// �^�C�����C���f�[�^
	vector<CameraKeyFrame*> data;
	if (j.contains("timeline") && j["timeline"].is_array()) {
		for (const auto& item : j["timeline"]) {
			CameraKeyFrame* keyframe = new CameraKeyFrame{
			item["time"],
			item["target"],
			GSvector3{ item["position"][0], item["position"][1], item["position"][2] },
			GSvector3{ item["lookat"][0], item["lookat"][1], item["lookat"][2] },
			item["angle"]
			};
			data.push_back(keyframe);
		}
	}

	return new CameraTimelineData{ data, start, end };
}

void CameraTimeline::add(const string& name, CameraTimelineData* data) {
	if (data == nullptr) return;

	// ���ɑ��݂��邩�ǂ���
	CameraTimelineData* current_data = find(name);
	// ���݂��Ȃ�
	if (current_data == nullptr) {
		timelines_[name] = data;
	}
	// ���݂���
	else {
		// �Đ����Ȃ�I��
		if (current_ == current_data) end();
		current_data->clear();	// �����̂�����	
		timelines_[name] = data;
	}
}

CameraTimeline::CameraTimelineData* CameraTimeline::find(const string& name) {
	auto it = timelines_.find(name);
	if (it != timelines_.end()) {
		return it->second;
	}
	return nullptr;
}

CameraTimeline::CameraTimelineData::CameraTimelineData(const vector<CameraKeyFrame*>& timeline, float start_transition_time, float end_transition_time) {
	timeline_ = timeline;
	start_transition_time_ = start_transition_time;
	end_transition_time_ = end_transition_time;
}

CameraTimeline::CameraTimelineData::~CameraTimelineData() {
	clear();
}

void CameraTimeline::CameraTimelineData::clear() {
	for (auto i : timeline_) {
		delete i;
		i = nullptr;
	}
	timeline_.clear();
}

const vector<CameraTimeline::CameraKeyFrame*>& CameraTimeline::CameraTimelineData::get() {
	return timeline_;
}

float CameraTimeline::CameraTimelineData::start_transition_time() const {
	return start_transition_time_;
}

float CameraTimeline::CameraTimelineData::end_transition_time() const {
	return end_transition_time_;
}
