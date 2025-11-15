#include "Engine/Core/Timeline/Parameters/SendMessageTimelineParameter.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/Actor/Actor.h"
#include "Engine/Utils/Check.h"
#include <any>

SendMessageTimelineParameter::SendMessageTimelineParameter(World* world) :
    ITimelineParameter(world) {

}

SendMessageTimelineParameter::~SendMessageTimelineParameter() {
    clear();
}

void SendMessageTimelineParameter::update(float delta_time) {
    // 再生中でなければ終了
    if (!is_playing_) {
        return;
    }

    // playの後にupdateが呼ばれる都合上先に処理
    SendMessageTimelineKeyFrame* current{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_, &current);
    SendMessageTimelineKeyFrame* next{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_ + 1, &next);

    // 次のキーフレームがなくなったら終了
    if (current == nullptr || next == nullptr) {
        stop();
        return;
    }

    auto send_message = [=](const string& target, const string& message) {
        Actor* actor = world_->find_actor(target);
        // もしアクターに登録されていなければフィールドアクターも検索をかける
        if (actor == nullptr) actor = world_->get_field()->find(target);
        if (actor != nullptr) {
            std::any data = message;
            actor->message("ByTimeline", data);
        }
        ++key_frame_;
    };

    // 最初のキーフレームを再生中かつ最初のキーフレーム開始時間が始まっていなかったらタイマーを更新して終了
    if (key_frame_ == 0 && current->time > timer_) {
        timer_ += delta_time / cFPS;
        if (current->time <= timer_) {
            send_message(current->target, current->message);
            return;
        }
        return;
    }

    // キーフレームを再生
    if (current->time >= timer_) {
        send_message(current->target, current->message);
    }

    // 再生キーフレームを更新
    timer_ += delta_time / cFPS;
}

void SendMessageTimelineParameter::clear() {
    stop();

    for (auto i : timelines_) {
        i.second->clear();
        delete i.second;
        i.second = nullptr;
    }
    timelines_.clear();
}

std::string SendMessageTimelineParameter::name() const {
    return "SendMessage";
}

void SendMessageTimelineParameter::load(const std::string& name, const json& j) {
    add(name, create_data(j));
}

void SendMessageTimelineParameter::play(const std::string& name) {
    play(find(name));
}

void SendMessageTimelineParameter::stop() {
    is_playing_ = false;
    current_ = nullptr;
}

void SendMessageTimelineParameter::play(SendMessageTimelineData* data) {
    if (data == nullptr) return;

    // 再生中なら停止する
    if (is_playing_) stop();

    // 更新
    is_playing_ = true;
    current_ = data;
    timer_ = 0.0f;
    key_frame_ = 0;
}

SendMessageTimelineParameter::SendMessageTimelineData* SendMessageTimelineParameter::create_data(const json& j) {
    // タイムラインデータ
    vector<SendMessageTimelineKeyFrame*> data;
    if (j.contains("timeline") && j["timeline"].is_array()) {
        // キーフレームを全て取得する
        for (const auto& item : j["timeline"]) {
            SendMessageTimelineKeyFrame* keyframe = new SendMessageTimelineKeyFrame{
                item["time"],
                item["target"],
                item["message"]
            };
            data.push_back(keyframe);
        }
    }
    return new SendMessageTimelineData{ data };
}

void SendMessageTimelineParameter::add(const string& name, SendMessageTimelineData* data) {
    if (data == nullptr) return;

    // 既に存在するかどうか
    SendMessageTimelineData* current_data = find(name);
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

SendMessageTimelineParameter::SendMessageTimelineData* SendMessageTimelineParameter::find(const std::string& name) {
    auto it = timelines_.find(name);
    if (it != timelines_.end()) {
        return it->second;
    }
    return nullptr;
}

#pragma region [キーフレーム管理を行うタイムラインデータクラス]
SendMessageTimelineParameter::SendMessageTimelineData::SendMessageTimelineData(const std::vector<SendMessageTimelineKeyFrame*>& timeline) {
    timeline_ = timeline;
}

SendMessageTimelineParameter::SendMessageTimelineData::~SendMessageTimelineData() {
    clear();
}

void SendMessageTimelineParameter::SendMessageTimelineData::clear() {
    for (auto i : timeline_) {
        delete i;
        i = nullptr;
    }
    timeline_.clear();
}

std::vector<SendMessageTimelineKeyFrame*>& SendMessageTimelineParameter::SendMessageTimelineData::get() {
    return timeline_;
}
#pragma endregion
