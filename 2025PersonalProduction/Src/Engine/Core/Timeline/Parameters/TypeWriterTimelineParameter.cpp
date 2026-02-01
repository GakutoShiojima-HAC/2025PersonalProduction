#include "TypeWriterTimelineParameter.h"
#include "Engine/Core/World/World.h"
#include "Engine/Utils/Check.h"

TypeWriterTimelineParameter::TypeWriterTimelineParameter(World* world) :
    ITimelineParameter(world) {

}

TypeWriterTimelineParameter::~TypeWriterTimelineParameter() {
    clear();
}

void TypeWriterTimelineParameter::update(float delta_time) {
    // 再生中でなければ終了
    if (!is_playing_) {
        return;
    }

    // playの後にupdateが呼ばれる都合上先に処理
    TypeWriterTimelineKeyFrame* current{ nullptr };
    MyLib::is_valid_index(current_->get(), key_frame_, &current);

    // 次のキーフレームがなくなったら終了
    if (current == nullptr) {
        stop();
        return;
    }

    auto react = [=]() {
        world_->set_type_writer(current->text, TextCode::UTF8);
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
    if (current->time <= timer_) {
        react();
    }

    // 再生キーフレームを更新
    timer_ += delta_time / cFPS;
}

void TypeWriterTimelineParameter::clear() {
    stop();

    for (auto i : timelines_) {
        i.second->clear();
        delete i.second;
        i.second = nullptr;
    }
    timelines_.clear();
}

std::string TypeWriterTimelineParameter::name() const {
    return "TypeWriter";
}

void TypeWriterTimelineParameter::load(const std::string& name, const json& j) {
    add(name, create_data(j));
}

void TypeWriterTimelineParameter::play(const std::string& name) {
    play(find(name));
}

void TypeWriterTimelineParameter::stop() {
    is_playing_ = false;
    current_ = nullptr;
    timer_ = 0.0f;
    key_frame_ = 0;
}

void TypeWriterTimelineParameter::play(TypeWriterTimelineData* data) {
    if (data == nullptr) return;

    // 再生中なら停止する
    if (is_playing_) stop();

    // 更新
    is_playing_ = true;
    current_ = data;
    timer_ = 0.0f;
    key_frame_ = 0;
}

TypeWriterTimelineParameter::TypeWriterTimelineData* TypeWriterTimelineParameter::create_data(const json& j) {
    // タイムラインデータ
    vector<TypeWriterTimelineKeyFrame*> data;
    if (j.contains("timeline") && j["timeline"].is_array()) {
        // キーフレームを全て取得する
        for (const auto& item : j["timeline"]) {
            std::vector<std::string> text;
            if (item.contains("text")) text = item["text"].get<std::vector<std::string>>();

            TypeWriterTimelineKeyFrame* keyframe = new TypeWriterTimelineKeyFrame{
                item["time"],
                text
            };
            data.push_back(keyframe);
        }
    }
    return new TypeWriterTimelineData{ data };
}

void TypeWriterTimelineParameter::add(const string& name, TypeWriterTimelineData* data) {
    if (data == nullptr) return;

    // 既に存在するかどうか
    TypeWriterTimelineData* current_data = find(name);
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

TypeWriterTimelineParameter::TypeWriterTimelineData* TypeWriterTimelineParameter::find(const std::string& name) {
    auto it = timelines_.find(name);
    if (it != timelines_.end()) {
        return it->second;
    }
    return nullptr;
}

#pragma region [キーフレーム管理を行うタイムラインデータクラス]
TypeWriterTimelineParameter::TypeWriterTimelineData::TypeWriterTimelineData(const std::vector<TypeWriterTimelineKeyFrame*>& timeline) {
    timeline_ = timeline;
}

TypeWriterTimelineParameter::TypeWriterTimelineData::~TypeWriterTimelineData() {
    clear();
}

void TypeWriterTimelineParameter::TypeWriterTimelineData::clear() {
    for (auto i : timeline_) {
        delete i;
        i = nullptr;
    }
    timeline_.clear();
}

std::vector<TypeWriterTimelineKeyFrame*>& TypeWriterTimelineParameter::TypeWriterTimelineData::get() {
    return timeline_;
}
#pragma endregion
