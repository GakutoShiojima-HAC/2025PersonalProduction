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
    // 再生中でなければ終了
    if (!is_playing_) {
        return;
    }
    // 再生中なのに必要なデータが無ければ終了
    if (current_ == nullptr || camera_ == nullptr) {
        stop();
        return;
    }

    // playの後にupdateが呼ばれる都合上先に処理
    CameraTimelineKeyFrame* current{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_, &current);
    CameraTimelineKeyFrame* next{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_ + 1, &next);

    // 次のキーフレームがなくなったら終了
    if (current == nullptr || next == nullptr) {
        stop();
        return;
    }
    // 最初のキーフレームを再生中かつ最初のキーフレーム開始時間が始まっていなかったらタイマーを更新して終了
    if (key_frame_ == 0 && current->time > timer_) {
        timer_ += delta_time / cFPS;
        if (current->time <= timer_) {
            world_->camera_transition(camera_, current_->start_transition_time());
        }
        return;
    }
    // 開始フレームならカメラを切り替える
    if (key_frame_ == 0 && current->time >= timer_) {
        world_->camera_transition(camera_, current_->start_transition_time());
    }

    // カメラパラメータを更新
    const float progress = CLAMP((timer_ - current->time) / (next->time - current->time), 0.0f, 1.0f);
    Actor* current_target = world_->find_actor(current->target);
    Actor* next_target = world_->find_actor(next->target);

    auto get_position = [](Actor* target, const GSvector3& local_position) {
        if (target == nullptr) return local_position;
        // 指定されたTranslate, Rotation, Scaleの行列を作成する
        GSmatrix4 local_matrix = GSmatrix4::TRS(local_position, GSquaternion::euler(GSvector3{ 0.0f, 0.0f, 0.0f }), GSvector3{ 1.0f, 1.0f, 1.0f });
        return (local_matrix * target->transform().localToWorldMatrix()).position();
        };

    // 座標
    const GSvector3 current_position = get_position(current_target, current->position);
    const GSvector3 next_position = get_position(next_target, next->position);
    const GSvector3 position = GSvector3::lerp(current_position, next_position, progress);
    camera_->transform().position(position);
    // 注視点
    const GSvector3 current_lookat = get_position(current_target, current->lookat);
    const GSvector3 next_lookat = get_position(next_target, next->lookat);
    const GSvector3 lookat = GSvector3::lerp(current_lookat, next_lookat, progress);
    camera_->transform().lookAt(lookat, GSvector3::up());
    // 角度
    auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
    camera_->transform().rotation(camera_->transform().rotation() * GSquaternion::angleAxis(lerp(current->angle, next->angle, progress), GSvector3::forward()));

    // 再生キーフレームを更新
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
    // 管理下に追加
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
    current_ = nullptr;
    timer_ = 0.0f;
    key_frame_ = 0;

    printf("STOP!\n");
}

void CameraTimelineParameter::play(CameraTimelineData* data) {
    if (data == nullptr) return;

    // 再生中なら停止する
    if (is_playing_) stop();

    // 更新
    is_playing_ = true;
    current_ = data;
    timer_ = 0.0f;
    key_frame_ = 0;
    prev_camera_ = world_->get_camera();
    camera_ = world_->find_camera(CameraTag::Timeline);
}

CameraTimelineParameter::CameraTimelineData* CameraTimelineParameter::create_data(const json& j) {
    // タイムライン用カメラに遷移するまでの時間
    const float start = MyJson::get_float(j, "start");
    // 元のカメラに遷移するまでの時間
    const float end = MyJson::get_float(j, "end");
    // タイムラインデータ
    vector<CameraTimelineKeyFrame*> data;
    if (j.contains("timeline") && j["timeline"].is_array()) {
        // キーフレームを全て取得する
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

    // 既に存在するかどうか
    CameraTimelineData* current_data = find(name);
    // 存在しない
    if (current_data == nullptr) {
        timelines_[name] = data;
    }
    // 存在する
    else {
        // 再生中なら終了
        if (current_ == current_data) stop();
        // 既存のを消去
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

#pragma region [キーフレーム管理を行うタイムラインデータクラス]
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
