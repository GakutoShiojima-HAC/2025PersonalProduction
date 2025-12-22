#include "PlayBGMTimelineEditor.h"
#include "Engine/Utils/MyString.h"

// 参照返しのエラー回避用
static float EMPTY_TIME{ 0.0f };

PlayBGMTimelineEditor::PlayBGMTimelineEditor(PlayBGMTimelineParameter& parameter) :
    parameter_{ parameter } {

}

PlayBGMTimelineEditor::~PlayBGMTimelineEditor() {
    clear();
}

void PlayBGMTimelineEditor::clear() {
    if (data_ != nullptr) data_->clear();
    delete data_;
    data_ = nullptr;
    edit_keyframe_index_ = 0;
}

void PlayBGMTimelineEditor::update_select_keyframe() {
    if (data_ == nullptr) return;
    vector<PlayBGMTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;
    PlayBGMTimelineKeyFrame* key_frame = timeline[edit_keyframe_index_];
    if (key_frame == nullptr) return;

    // 時間を編集
    ImGui::PushItemWidth(80);
    if (ImGui::InputFloat(ToUTF8("キーフレームの時間").c_str(), &key_frame->time)) {
        sort_timeline();
        auto it = find(timeline.begin(), timeline.end(), key_frame);
        if (it != timeline.end()) edit_keyframe_index_ = distance(timeline.begin(), it);
    }
    ImGui::PopItemWidth();

    // BGM番号の名前を編集
    ImGui::PushItemWidth(200);
    ImGui::InputText(ToUTF8("BGMの登録名").c_str(), &key_frame->bgm_name);
    ImGui::PopItemWidth();

    // フェード時間を編集
    ImGui::PushItemWidth(200);
    ImGui::InputFloat(ToUTF8("前のBGMのフェードアウト時間").c_str(), &key_frame->transition_time);
    ImGui::PopItemWidth();

    // キーフレームを削除
    if (ImGui::Button(ToUTF8("選択中のキーフレームを削除").c_str())) remove_keyframe(edit_keyframe_index_);
}

std::string PlayBGMTimelineEditor::name() const {
    return parameter_.name();
}

float PlayBGMTimelineEditor::play_time() const {
    return parameter_.play_time();
}

bool PlayBGMTimelineEditor::is_empty() const {
    if (data_ == nullptr) return true;
    return data_->get().empty();
}

unsigned int PlayBGMTimelineEditor::count_keyframe() const {
    if (data_ == nullptr) return 0;
    return data_->get().size();
}

float& PlayBGMTimelineEditor::get_keyframe_time(unsigned int index) {
    if (data_ == nullptr) return EMPTY_TIME;
    std::vector<PlayBGMTimelineKeyFrame*>& data = data_->get();
    if (data.empty() || index >= data.size()) return EMPTY_TIME;
    return data[index]->time;
}

void PlayBGMTimelineEditor::sort_timeline() {
    if (data_ == nullptr) return;
    vector<PlayBGMTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;

    sort(timeline.begin(), timeline.end(), [](const PlayBGMTimelineKeyFrame* a, const PlayBGMTimelineKeyFrame* b) {
        return a->time < b->time;  // 昇順にソート
    });
}

void PlayBGMTimelineEditor::add_keyframe(float time) {
    // キーフレームを作成
    PlayBGMTimelineKeyFrame* keyframe = new PlayBGMTimelineKeyFrame{ time, "", 0.0f };

    // データが無ければ作成
    if (data_ == nullptr) {
        std::vector<PlayBGMTimelineKeyFrame*> timeline;
        data_ = new PlayBGMTimelineParameter::PlayBGMTimelineData(timeline);
    }

    vector<PlayBGMTimelineKeyFrame*>& timeline = data_->get();
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
            const PlayBGMTimelineKeyFrame* lhs,
            const PlayBGMTimelineKeyFrame* rhs) {
                return lhs->time < rhs->time;
        }
    );
    timeline.insert(it, keyframe);
}

void PlayBGMTimelineEditor::remove_keyframe(unsigned int index) {
    if (data_ == nullptr) return;
    vector<PlayBGMTimelineKeyFrame*>& timeline = data_->get();

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

ordered_json PlayBGMTimelineEditor::save_data() {
    ordered_json data;

    if (data_ == nullptr || data_->get().empty()) return data;

    for (const auto* kf : data_->get()) {
        ordered_json item;
        item["time"] = kf->time;
        item["name"] = kf->bgm_name;
        item["transition"] = kf->transition_time;
        data["timeline"].push_back(item);
    }
    return data;
}

void PlayBGMTimelineEditor::load(const json& j) {
    clear();
    data_ = parameter_.create_data(j);
}

void PlayBGMTimelineEditor::play() {
    if (data_ == nullptr) return;
    parameter_.play(data_);
}
