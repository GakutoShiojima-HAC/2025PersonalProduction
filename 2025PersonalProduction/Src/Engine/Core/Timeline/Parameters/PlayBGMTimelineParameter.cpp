#include "PlayBGMTimelineParameter.h"
#include "Engine/Core/World/World.h"
#include "Engine/Utils/Check.h"
#include "Assets.h"

PlayBGMTimelineParameter::PlayBGMTimelineParameter(World* world) :
    ITimelineParameter(world) {

}

PlayBGMTimelineParameter::~PlayBGMTimelineParameter() {
    clear();
}

void PlayBGMTimelineParameter::update(float delta_time) {
    // 再生中でなければ終了
    if (!is_playing_) {
        return;
    }

    // playの後にupdateが呼ばれる都合上先に処理
    PlayBGMTimelineKeyFrame* current{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_, &current);

    // 次のキーフレームがなくなったら終了
    if (current == nullptr) {
        stop();
        return;
    }

    auto react = [=]() {
        if (current->transition_time > 0.0f) manager_.play(Assets::to_bgm_id(current->bgm_name));
        else manager_.play(Assets::to_bgm_id(current->bgm_name), current->transition_time);
        ++key_frame_;
    };

    // 最初のキーフレームを再生中かつ最初のキーフレーム開始時間が始まっていなかったらタイマーを更新して終了
    if (key_frame_ == 0 && current->time > timer_) {
        timer_ += delta_time / cFPS;
        if (current->time <= timer_) {
            react();
            return;
        }
        return;
    }

    // キーフレームを再生
    if (current->time >= timer_) {
        react();
    }

    // 再生キーフレームを更新
    timer_ += delta_time / cFPS;
}

void PlayBGMTimelineParameter::clear() {
    stop();

    for (auto i : timelines_) {
        i.second->clear();
        delete i.second;
        i.second = nullptr;
    }
    timelines_.clear();
}

std::string PlayBGMTimelineParameter::name() const {
    return "PlayBGM";
}

void PlayBGMTimelineParameter::load(const std::string& name, const json& j) {
    add(name, create_data(j));
}

void PlayBGMTimelineParameter::play(const std::string& name) {
    play(find(name));
}

void PlayBGMTimelineParameter::stop() {
    is_playing_ = false;
    current_ = nullptr;
}

void PlayBGMTimelineParameter::play(PlayBGMTimelineData* data) {
    if (data == nullptr) return;

    // 再生中なら停止する
    if (is_playing_) stop();

    // 更新
    is_playing_ = true;
    current_ = data;
    timer_ = 0.0f;
    key_frame_ = 0;
}

PlayBGMTimelineParameter::PlayBGMTimelineData* PlayBGMTimelineParameter::create_data(const json& j) {
    // タイムラインデータ
    vector<PlayBGMTimelineKeyFrame*> data;
    if (j.contains("timeline") && j["timeline"].is_array()) {
        // キーフレームを全て取得する
        for (const auto& item : j["timeline"]) {
            PlayBGMTimelineKeyFrame* keyframe = new PlayBGMTimelineKeyFrame{
                item["time"],
                item["name"],
                item["transition"]
            };
            data.push_back(keyframe);
        }
    }
    return new PlayBGMTimelineData{ data };
}

void PlayBGMTimelineParameter::add(const string& name, PlayBGMTimelineData* data) {
    if (data == nullptr) return;

    // 既に存在するかどうか
    PlayBGMTimelineData* current_data = find(name);
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

PlayBGMTimelineParameter::PlayBGMTimelineData* PlayBGMTimelineParameter::find(const std::string& name) {
    auto it = timelines_.find(name);
    if (it != timelines_.end()) {
        return it->second;
    }
    return nullptr;
}

#pragma region [キーフレーム管理を行うタイムラインデータクラス]
PlayBGMTimelineParameter::PlayBGMTimelineData::PlayBGMTimelineData(const std::vector<PlayBGMTimelineKeyFrame*>& timeline) {
    timeline_ = timeline;
}

PlayBGMTimelineParameter::PlayBGMTimelineData::~PlayBGMTimelineData() {
    clear();
}

void PlayBGMTimelineParameter::PlayBGMTimelineData::clear() {
    for (auto i : timeline_) {
        delete i;
        i = nullptr;
    }
    timeline_.clear();
}

std::vector<PlayBGMTimelineKeyFrame*>& PlayBGMTimelineParameter::PlayBGMTimelineData::get() {
    return timeline_;
}
#pragma endregion
