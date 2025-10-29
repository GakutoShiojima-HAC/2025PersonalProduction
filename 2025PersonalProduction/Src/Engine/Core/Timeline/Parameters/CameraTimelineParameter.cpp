#include "CameraTimelineParameter.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Actor/Actor.h"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Utils/Check.h"

CameraTimelineParameter::CameraTimelineParameter(World* world) :
    ITimelineParameter(world) {

}

CameraTimelineParameter::~CameraTimelineParameter() {
    clear();
}

void CameraTimelineParameter::update(float delta_time) {
    // �Đ����łȂ���ΏI��
    if (!is_playing_) {
        return;
    }
    // �Đ����Ȃ̂ɕK�v�ȃf�[�^��������ΏI��
    if (current_ == nullptr || camera_ == nullptr) {
        stop();
        return;
    }

    // play�̌��update���Ă΂��s�����ɏ���
    CameraTimelineKeyFrame* current{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_, &current);
    CameraTimelineKeyFrame* next{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_ + 1, &next);

    // ���̃L�[�t���[�����Ȃ��Ȃ�����I��
    if (current == nullptr || next == nullptr) {
        stop();
        return;
    }
    // �ŏ��̃L�[�t���[�����Đ������ŏ��̃L�[�t���[���J�n���Ԃ��n�܂��Ă��Ȃ�������^�C�}�[���X�V���ďI��
    if (key_frame_ == 0 && current->time > timer_) {
        timer_ += delta_time / cFPS;
        if (current->time <= timer_) {
            world_->camera_transition(camera_, current_->start_transition_time());
        }
        return;
    }
    // �J�n�t���[���Ȃ�J������؂�ւ���
    if (key_frame_ == 0 && current->time >= timer_) {
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
        ++key_frame_;
    }
}

void CameraTimelineParameter::clear() {
    stop();

    for (auto i : timelines_) {
        i.second->clear();
        delete i.second;
        i.second = nullptr;
    }
    timelines_.clear();
}

std::string CameraTimelineParameter::name() const {
    return "Camera";
}

void CameraTimelineParameter::load(const std::string& name, const json& j) {
    // �Ǘ����ɒǉ�
    add(name, create_data(j));
}

void CameraTimelineParameter::play(const std::string& name) {
    play(find(name));
}

void CameraTimelineParameter::stop() {
    if (prev_camera_ != nullptr) world_->camera_transition(prev_camera_, current_ != nullptr ? current_->end_transition_time() : 0.0f);
    is_playing_ = false;
    current_ = nullptr;
    camera_ = nullptr;
    prev_camera_ = nullptr;
    
}

void CameraTimelineParameter::play(CameraTimelineData* data) {
    if (data == nullptr) return;

    // �Đ����Ȃ��~����
    if (is_playing_) stop();

    // �X�V
    is_playing_ = true;
    current_ = data;
    timer_ = 0.0f;
    key_frame_ = 0;
    prev_camera_ = world_->get_camera();
    camera_ = world_->find_camera(CameraTag::Timeline);
}

CameraTimelineParameter::CameraTimelineData* CameraTimelineParameter::create_data(const json& j) {
    // �^�C�����C���p�J�����ɑJ�ڂ���܂ł̎���
    const float start = MyJson::get_float(j, "start");
    // ���̃J�����ɑJ�ڂ���܂ł̎���
    const float end = MyJson::get_float(j, "end");
    // �^�C�����C���f�[�^
    vector<CameraTimelineKeyFrame*> data;
    if (j.contains("timeline") && j["timeline"].is_array()) {
        // �L�[�t���[����S�Ď擾����
        for (const auto& item : j["timeline"]) {
            CameraTimelineKeyFrame* keyframe = new CameraTimelineKeyFrame{
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

void CameraTimelineParameter::add(const string& name, CameraTimelineParameter::CameraTimelineData* data) {
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
        if (current_ == current_data) stop();
        // �����̂�����
        current_data->clear();
        delete current_data;
        timelines_[name] = data;
    }
}

CameraTimelineParameter::CameraTimelineData* CameraTimelineParameter::find(const std::string& name) {
    auto it = timelines_.find(name);
    if (it != timelines_.end()) {
        return it->second;
    }
    return nullptr;
}

#pragma region [�L�[�t���[���Ǘ����s���^�C�����C���f�[�^�N���X]
CameraTimelineParameter::CameraTimelineData::CameraTimelineData(const std::vector<CameraTimelineKeyFrame*>& timeline, float start_transition_time, float end_transition_time) {
    timeline_ = timeline;
    start_transition_time_ = start_transition_time;
    end_transition_time_ = end_transition_time;
}

CameraTimelineParameter::CameraTimelineData::~CameraTimelineData() {
    clear();
}

void CameraTimelineParameter::CameraTimelineData::clear() {
    for (auto i : timeline_) {
        delete i;
        i = nullptr;
    }
    timeline_.clear();
}

vector<CameraTimelineKeyFrame*>& CameraTimelineParameter::CameraTimelineData::get() {
    return timeline_;
}

float& CameraTimelineParameter::CameraTimelineData::start_transition_time() {
    return start_transition_time_;
}

float& CameraTimelineParameter::CameraTimelineData::end_transition_time() {
    return end_transition_time_;
}
#pragma endregion
