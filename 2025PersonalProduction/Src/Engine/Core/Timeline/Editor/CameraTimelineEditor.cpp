#include "CameraTimelineEditor.h"

// �Q�ƕԂ��̃G���[���p
static float EMPTY_TIME{ 0.0f };

CameraTimelineEditor::CameraTimelineEditor(CameraTimelineParameter& parameter) :
    parameter_{ parameter } {

}

CameraTimelineEditor::~CameraTimelineEditor() {
    clear();
}

void CameraTimelineEditor::clear() {
    if (data_ != nullptr) data_->clear();
    delete data_;
    data_ = nullptr;
    edit_keyframe_index_ = 0;
}

void CameraTimelineEditor::update_select_keyframe() {
    if (data_ == nullptr) return;
    vector<CameraTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;
    CameraTimelineKeyFrame* key_frame = timeline[edit_keyframe_index_];
    if (key_frame == nullptr) return;

    // �J�����L�����Ɩ��������̑J�ڎ��Ԃ�ݒ�
    ImGui::PushItemWidth(80);
    ImGui::InputFloat("start transition time", &data_->start_transition_time());
    ImGui::PopItemWidth();

    // �J�����I���̑J�ڎ��Ԃ̐ݒ�
    ImGui::SameLine();
    ImGui::PushItemWidth(80);
    ImGui::InputFloat("end transition time", &data_->end_transition_time());
    ImGui::PopItemWidth();

    // ���Ԃ�ҏW
    ImGui::PushItemWidth(80);
    if (ImGui::InputFloat("time##2", &key_frame->time)) {
        sort_timeline();
        auto it = find(timeline.begin(), timeline.end(), key_frame);
        if (it != timeline.end()) edit_keyframe_index_ = distance(timeline.begin(), it);
    }
    ImGui::PopItemWidth();

    // �^�[�Q�b�g��ҏW
    ImGui::SameLine();
    ImGui::PushItemWidth(200);
    ImGui::InputText("origin target##2", &key_frame->target);
    ImGui::PopItemWidth();

    // ���W��ҏW
    ImGui::PushItemWidth(200);
    ImGui::DragFloat3("position", key_frame->position, 0.1f);
    // �����_������ҏW
    ImGui::DragFloat3("lookat", key_frame->lookat, 0.1f);
    // �X����ҏW
    ImGui::DragFloat("angle", &key_frame->angle, 0.1f);
    ImGui::PopItemWidth();

    // ���݂̃J�����̍��W��K�p
    if (ImGui::Button("Apply Current Camera Location")) {
        Camera* camera = parameter_.get_world()->get_camera();
        Actor* target = parameter_.get_world()->find_actor(key_frame->target);
        if (camera != nullptr) {
            // �^�[�Q�b�g�����݂���Ȃ瑊�΍��W�����
            GSvector3 position = camera->transform().position();
            GSvector3 lookat = position + camera->transform().forward();
            if (target != nullptr) position = target->transform().inverseTransformPoint(position);
            if (target != nullptr) lookat = target->transform().inverseTransformPoint(lookat);
            key_frame->position = position;
            key_frame->lookat = lookat;
            key_frame->angle = get_tilt_angle(camera->transform().rotation());
        }
    }

    // �L�[�t���[�����폜
    ImGui::SameLine();
    if (ImGui::Button("Delete KeyFrame")) remove_keyframe(edit_keyframe_index_);
}

std::string CameraTimelineEditor::name() const {
    return parameter_.name();
}

bool CameraTimelineEditor::is_empty() const {
    if (data_ == nullptr) return true;
    return data_->get().empty();
}

unsigned int CameraTimelineEditor::count_keyframe() const {
    if (data_ == nullptr) return 0;
    return data_->get().size();
}

float& CameraTimelineEditor::get_keyframe_time(unsigned int index) {
    if (data_ == nullptr) return EMPTY_TIME;
    std::vector<CameraTimelineKeyFrame*>& data = data_->get();
    if (data.empty() || index >= data.size()) return EMPTY_TIME;
    return data[index]->time;
}

void CameraTimelineEditor::sort_timeline() {
    if (data_ == nullptr) return;
    vector<CameraTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;

    sort(timeline.begin(), timeline.end(), [](const CameraTimelineKeyFrame* a, const CameraTimelineKeyFrame* b) {
        return a->time < b->time;  // �����Ƀ\�[�g
    });
}

void CameraTimelineEditor::add_keyframe(float time) {
    // �L�[�t���[�����쐬
    CameraTimelineKeyFrame* keyframe = new CameraTimelineKeyFrame{ time, "", GSvector3{ 0.0f, 0.0f, 0.0f }, GSvector3{ 0.0f, 0.0f, 0.0f }, 0.0f };

    // �f�[�^��������΍쐬
    if (data_ == nullptr) {
        std::vector<CameraTimelineKeyFrame*> timeline;
        data_ = new CameraTimelineParameter::CameraTimelineData(timeline, 0.0f, 0.0f);
    }

    vector<CameraTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) {
        timeline.push_back(keyframe);
        edit_keyframe_index_ = 0;
        return;
    }

    auto it = lower_bound(
        timeline.begin(),
        timeline.end(),
        keyframe,
        [](
            const CameraTimelineKeyFrame* lhs,
            const CameraTimelineKeyFrame* rhs) {
                return lhs->time < rhs->time;
        }
    );
    timeline.insert(it, keyframe);
}

void CameraTimelineEditor::remove_keyframe(unsigned int index) {
    if (data_ == nullptr) return;
    vector<CameraTimelineKeyFrame*>& timeline = data_->get();

    if (index < timeline.size()) {
        delete timeline[index];
        timeline.erase(timeline.begin() + index);
    }
    if (timeline.size() == 0) {
        timeline.clear();
    }
    if (edit_keyframe_index_ >= timeline.size()) {
        int index = CLAMP(timeline.size() - 1, 0, INT_MAX);
        edit_keyframe_index_ = index;
    }
}

ordered_json CameraTimelineEditor::save_data() {
    ordered_json data;
    data["start"] = data_->start_transition_time();
    data["end"] = data_->end_transition_time();

    for (const auto* kf : data_->get()) {
        ordered_json item;
        item["time"] = kf->time;
        item["target"] = kf->target;
        item["position"] = { kf->position.x, kf->position.y, kf->position.z };
        item["lookat"] = { kf->lookat.x, kf->lookat.y, kf->lookat.z };
        item["angle"] = kf->angle;
        data["timeline"].push_back(item);
    }
    return data;
}

void CameraTimelineEditor::load(const json& j) {
    clear();
    data_ = parameter_.create_data(j);
}

void CameraTimelineEditor::play() {
    if (data_ == nullptr) return;
    parameter_.play(data_);
}

float CameraTimelineEditor::get_tilt_angle(const GSquaternion& rotation) const {
    GSvector3 up = rotation * GSvector3::up();              // �J�����̏����
    GSvector3 forward = rotation * GSvector3::forward();    // �J�����̑O����

    GSvector3 right = GSvector3::cross(GSvector3::up(), forward).normalized();  // ��E
    GSvector3 base_up = GSvector3::cross(forward, right);                       // ���

    // �J�����Ɗ�𓊉e���ʏ�Ōv�Z
    float angle = std::atan2(GSvector3::dot(GSvector3::cross(base_up, up), forward), GSvector3::dot(base_up, up));
    return angle * (180.0f / GS_PI);
}
