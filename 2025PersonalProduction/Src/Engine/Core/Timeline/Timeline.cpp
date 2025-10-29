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

    // タイムラインパラメータを追加
    CameraTimelineParameter* camera_timeline = new CameraTimelineParameter(world);
    timelines_.push_back(camera_timeline);

    if (!enable_editor) return;
    // エディタを追加
    editor_.add(new CameraTimelineEditor(*camera_timeline));
}

void Timeline::load(const std::string& json_file) {
    json j;
    if (!MyJson::is_json(json_file, j)) return;

    // 読み込むタイムラインデータを全て回す
    for (auto it = j.begin(); it != j.end(); ++it) {
        // データがjsonかどうか
        const std::string data_path = it.value();
        json data_json;
        if (!MyJson::is_json(data_path, data_json)) continue;
        // 再生用のデータ名を取得
        const std::string data_name = MyJson::get_string(data_json, "Name");
        // 定義されているパラメータを全て回す
        for (auto it = data_json.begin(); it != data_json.end(); ++it) {
            const std::string parameter_name = it.key();
            ITimelineParameter* parameter = find(parameter_name);
            if (parameter == nullptr) continue;

            // 対応するパラメータにオブジェクトを渡す
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

