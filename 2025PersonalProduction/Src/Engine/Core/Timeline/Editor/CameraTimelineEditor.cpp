#include "Engine/Core/Timeline/Editor/CameraTimelineEditor.h"
#include "Engine/Utils/Check.h"
#include "Engine/Core/Camera/Camera.h"
#include "Lib/json.hpp"
#include "Engine/Utils/Folder.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

const string PATH{ "Resource/Private/Timeline/Camera/" };

CameraTimelineEditor::CameraTimelineEditor(World* world) {
	world_ = world;

    timeline_ = new CameraTimelineData{ vector<CameraKeyFrame*>(), 0.0f, 0.0f };
}

CameraTimelineEditor::~CameraTimelineEditor() {
	clear();
}

void CameraTimelineEditor::update(float delta_time) {
    ImGui::Begin("Camera KeyFrame Inspector Window");

    const bool is_empty = timeline_ == nullptr || timeline_->get().empty();
    draw_main_gui(is_empty, PATH);
    draw_add_keyframe();
    draw_timeline(is_empty, is_empty ? 0 : timeline_->get().size());
    draw_current_keyframe();

    ImGui::End();
}

void CameraTimelineEditor::clear() {
    if (timeline_ != nullptr) timeline_->clear();
    delete timeline_;
    timeline_ = nullptr;
}

void CameraTimelineEditor::draw_add_keyframe() {
    if (timeline_ == nullptr) return;

    // カメラ開始の遷移時間の設定
    ImGui::Separator();
    ImGui::PushItemWidth(80);
    ImGui::InputFloat("start transition time", &timeline_->start_transition_time());
    ImGui::PopItemWidth();

    // カメラ終了の遷移時間の設定
    ImGui::SameLine();
    ImGui::PushItemWidth(80);
    ImGui::InputFloat("end transition time", &timeline_->end_transition_time());
    ImGui::PopItemWidth();

    // キーフレーム追加
    vector<CameraKeyFrame*>& timeline = timeline_->get();
    if (ImGui::Button("Add KeyFrame")) {
        Camera* camera = world_->get_camera();
        add_key_frame(
            time_,
            target_,
            camera->transform().position(),
            camera->transform().position() + camera->transform().forward(),
            get_tilt_angle(camera->transform().rotation())

        );
        set_keyframe(1, timeline.size());
    }

    // ターゲット名のリセット
    ImGui::SameLine();
    if (ImGui::Button("Reset Target")) target_ = "";

    // キーフレーム時間の設定
    ImGui::PushItemWidth(80);
    ImGui::InputFloat("time##1", &time_);
    ImGui::PopItemWidth();

    // 原点とするターゲット名の設定
    ImGui::SameLine();
    ImGui::PushItemWidth(200);
    ImGui::InputText("origin target##1", &target_);
    ImGui::PopItemWidth();    
}

void CameraTimelineEditor::draw_current_keyframe() {
    if (timeline_ == nullptr) return;
    vector<CameraKeyFrame*>& timeline = timeline_->get();
    if (timeline.empty()) return;
    CameraKeyFrame* key_frame = timeline[current_edit_keyframe_];
    if (key_frame == nullptr) return;

    // キーフレーム時間の設定
    ImGui::Separator();
    ImGui::PushItemWidth(80);
    if (ImGui::InputFloat("time##2", &key_frame->time)) {
        sort_timeline();
        auto it = find(timeline.begin(), timeline.end(), key_frame);
        if (it != timeline.end()) current_edit_keyframe_ = distance(timeline.begin(), it);
    }
    ImGui::PopItemWidth();

    // 原点とするターゲット名の設定
    ImGui::SameLine();
    ImGui::PushItemWidth(200);
    ImGui::InputText("origin target##2", &key_frame->target);

    // カメラパラメータの設定
    ImGui::DragFloat3("position", key_frame->position, 0.1f);
    ImGui::DragFloat3("lookat", key_frame->lookat, 0.1f);
    ImGui::DragFloat("angle", &key_frame->angle, 0.1f);
    ImGui::PopItemWidth();

    // 現在のカメラパラメータを適用
    if (ImGui::Button("Apply Current Camera Location")) {
        Camera* camera = world_->get_camera();
        if (camera != nullptr) {
            key_frame->position = camera->transform().position();
            key_frame->lookat = camera->transform().position() + camera->transform().forward();
            key_frame->angle = get_tilt_angle(camera->transform().rotation());
        }
    }

    // キーフレームを削除
    ImGui::SameLine();
    if (ImGui::Button("Delete KeyFrame")) remove_key_frame(current_edit_keyframe_);
}

void CameraTimelineEditor::play() {
    // マネージャーを取得
    CameraTimeline* camera_timeline = world_->timeline().camera_timeline();
    if (camera_timeline == nullptr) return;

    camera_timeline->play_data(timeline_);
}

void CameraTimelineEditor::save() {
    if (timeline_ == nullptr) return;
    vector<CameraKeyFrame*>& timeline = timeline_->get();
    if (timeline.empty()) return;

    sort_timeline();
    ordered_json j;
    j["start"] = timeline_->start_transition_time();
    j["end"] = timeline_->end_transition_time();

    for (const auto* kf : timeline) {
        ordered_json item;
        item["time"] = kf->time;
        item["target"] = kf->target;
        item["position"] = { kf->position.x, kf->position.y, kf->position.z };
        item["lookat"] = { kf->lookat.x, kf->lookat.y, kf->lookat.z };
        item["angle"] = kf->angle;
        j["timeline"].push_back(item);
    }

    // ファイルパス
    const string root{ "Resource/Private/Timeline/Camera" };
    // 上書き保存
    MyLib::create_folder(root);
    MyLib::write_to_file(root + "/" + save_filename_ + ".json", j);
    // 保存したのでエディタ上は破棄
    reset();
}

void CameraTimelineEditor::load() {
    // マネージャーを取得
    CameraTimeline* camera_timeline = world_->timeline().camera_timeline();
    if (camera_timeline == nullptr) return;

    // データを生成
    CameraTimelineData* data = camera_timeline->load("Resource/Private/Timeline/Camera/" + load_filename_ + ".json");
    load_filename_ = "";
    if (data == nullptr) return;

    clear();
    timeline_ = data;
    target_ = "";
    save_filename_ = "";
    time_ = timeline_->get().empty() ? 0.0f : timeline_->get()[timeline_->get().size() - 1]->time;
    current_edit_keyframe_ = 0;
}

void CameraTimelineEditor::reset() {
    timeline_->clear();
    timeline_->start_transition_time() = 0.0f;
    timeline_->end_transition_time() = 0.0f;
    save_filename_ = "";
    target_ = "";
    time_ = 0.0f;
    timeline_start_time_ = 0.0f;
    current_edit_keyframe_ = 0;
}

void CameraTimelineEditor::sort_timeline() {
    if (timeline_ == nullptr) return;
    vector<CameraKeyFrame*>& timeline = timeline_->get();
    if (timeline.empty()) return;

    sort(timeline.begin(), timeline.end(), [](const CameraKeyFrame* a, const CameraKeyFrame* b) {
        return a->time < b->time;  // 昇順にソート
    });
}

void CameraTimelineEditor::remove_key_frame(GSuint index) {
    if (timeline_ == nullptr) return;
    vector<CameraKeyFrame*>& timeline = timeline_->get();

    if (index < timeline.size()) {
        delete timeline[index];
        timeline.erase(timeline.begin() + index);
    }
    if (timeline.size() == 0) {
        timeline.clear();
    }
    if (current_edit_keyframe_ >= timeline.size()) {
        current_edit_keyframe_ = timeline.size() - 1;
    }
}

void CameraTimelineEditor::add_key_frame(float time, const string& target, const GSvector3& position, const GSvector3& lookat, float angle) {
    if (timeline_ == nullptr) return;
    
    CameraKeyFrame* key_frame = new CameraKeyFrame{ time, target, position, lookat, angle };

    vector<CameraKeyFrame*>& timeline = timeline_->get();
    if (timeline.empty()) {
        timeline.push_back(key_frame);
        return;
    }

    auto it = lower_bound(
        timeline.begin(),
        timeline.end(),
        key_frame,
        [](
            const CameraKeyFrame* lhs,
            const CameraKeyFrame* rhs) {
                return lhs->time < rhs->time;
        }
    );
    timeline.insert(it, key_frame);
}

float CameraTimelineEditor::get_tilt_angle(const GSquaternion& rotation) const {
    GSvector3 up = rotation * GSvector3::up();  // カメラの上方向
    GSvector3 forward = rotation * GSvector3::forward(); // カメラの前方向

    GSvector3 right = GSvector3::cross(GSvector3::up(), forward).normalized(); // 基準右
    GSvector3 base_up = GSvector3::cross(forward, right); // 基準上
    
    // カメラと基準を投影平面上で計算
    float angle = std::atan2(GSvector3::dot(GSvector3::cross(base_up, up), forward),GSvector3::dot(base_up, up));
    return angle * (180.0f / GS_PI);
}

float& CameraTimelineEditor::get_time(int index) {
    float tmp{ 0.0f };
    if (timeline_ == nullptr) return tmp;
    vector<CameraKeyFrame*>& timeline = timeline_->get();
    if (timeline.empty() || index >= (int)timeline.size()) return tmp;

    return timeline[index]->time;
}
