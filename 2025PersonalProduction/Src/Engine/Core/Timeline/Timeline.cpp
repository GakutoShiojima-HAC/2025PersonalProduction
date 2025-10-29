#include "Timeline.h"
#include "Parameters/ITimelineParameter.h"
#include "Engine/Utils/MyJson.h"
#include "TimelineEditor.h"

#include "Parameters/CameraTimelineParameter.h"
#include "Editor/CameraTimelineEditor.h"

Timeline::~Timeline() {
    clear();
}

void Timeline::update(float delta_time) {
    for (auto& param : timelines_) {
        param->update(delta_time);
    }
    editor_.update(delta_time);
}

void Timeline::clear() {
    editor_.clear();

    for (auto param : timelines_) {
        delete param;
        param = nullptr;
    }
    timelines_.clear();
}

void Timeline::init(World* world, bool enable_editor) {
    clear();

    // �^�C�����C���p�����[�^��ǉ�
    CameraTimelineParameter* camera_timeline = new CameraTimelineParameter(world);
    timelines_.push_back(camera_timeline);

    if (!enable_editor) return;
    // �G�f�B�^��ǉ�
    editor_.add(new CameraTimelineEditor(*camera_timeline));
}

void Timeline::load(const std::string& json_file) {
    json j;
    if (!MyJson::is_json(json_file, j)) return;

    // �ǂݍ��ރ^�C�����C���f�[�^��S�ĉ�
    for (auto it = j.begin(); it != j.end(); ++it) {
        // �f�[�^��json���ǂ���
        const std::string data_path = it.value();
        json data_json;
        if (!MyJson::is_json(data_path, data_json)) continue;
        // �Đ��p�̃f�[�^�����擾
        const std::string data_name = MyJson::get_string(data_json, "Name");
        // ��`����Ă���p�����[�^��S�ĉ�
        for (auto it = data_json.begin(); it != data_json.end(); ++it) {
            const std::string parameter_name = it.key();
            ITimelineParameter* parameter = find(parameter_name);
            if (parameter == nullptr) continue;

            // �Ή�����p�����[�^�ɃI�u�W�F�N�g��n��
            parameter->load(data_name, *it);
        }
    }
}

void Timeline::play(const std::string& name) {
    for (auto& param : timelines_) {
        param->play(name);
    }
}

void Timeline::stop() {
    for (auto& param : timelines_) {
        param->stop();
    }
}

ITimelineParameter* Timeline::find(const std::string& name) {
    for (auto& param : timelines_) {
        if (param->name() == name) return param;
    }
    return nullptr;
}

