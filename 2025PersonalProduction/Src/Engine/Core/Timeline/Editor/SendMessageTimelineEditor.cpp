#include "SendMessageTimelineEditor.h"
#include "Engine/Utils/MyString.h"

// 参照返しのエラー回避用
static float EMPTY_TIME{ 0.0f };

SendMessageTimelineEditor::SendMessageTimelineEditor(SendMessageTimelineParameter& parameter) :
    parameter_{ parameter } {

}

SendMessageTimelineEditor::~SendMessageTimelineEditor() {
    clear();
}

void SendMessageTimelineEditor::clear() {
    if (data_ != nullptr) data_->clear();
    delete data_;
    data_ = nullptr;
    edit_keyframe_index_ = 0;
}

void SendMessageTimelineEditor::update_select_keyframe() {
    if (data_ == nullptr) return;
    vector<SendMessageTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;
    SendMessageTimelineKeyFrame* key_frame = timeline[edit_keyframe_index_];
    if (key_frame == nullptr) return;

    // 時間を編集
    ImGui::PushItemWidth(80);
    if (ImGui::InputFloat(ToUTF8("キーフレームの時間").c_str(), &key_frame->time)) {
        sort_timeline();
        auto it = find(timeline.begin(), timeline.end(), key_frame);
        if (it != timeline.end()) edit_keyframe_index_ = distance(timeline.begin(), it);
    }
    ImGui::PopItemWidth();

    // ターゲットを編集
    ImGui::PushItemWidth(200);
    ImGui::InputText(ToUTF8("送信先のアクター名").c_str(), &key_frame->target);
    ImGui::PopItemWidth();

    // メッセージを編集
    ImGui::PushItemWidth(200);
    ImGui::InputText(ToUTF8("送信するメッセージ").c_str(), &key_frame->message);
    ImGui::PopItemWidth();

    // キーフレームを削除
    if (ImGui::Button(ToUTF8("選択中のキーフレームを削除").c_str())) remove_keyframe(edit_keyframe_index_);
}

std::string SendMessageTimelineEditor::name() const {
    return parameter_.name();
}

bool SendMessageTimelineEditor::is_empty() const {
    if (data_ == nullptr) return true;
    return data_->get().empty();
}

unsigned int SendMessageTimelineEditor::count_keyframe() const {
    if (data_ == nullptr) return 0;
    return data_->get().size();
}

float& SendMessageTimelineEditor::get_keyframe_time(unsigned int index)
{
    if (data_ == nullptr) return EMPTY_TIME;
    std::vector<SendMessageTimelineKeyFrame*>& data = data_->get();
    if (data.empty() || index >= data.size()) return EMPTY_TIME;
    return data[index]->time;
}

void SendMessageTimelineEditor::sort_timeline() {
    if (data_ == nullptr) return;
    vector<SendMessageTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;

    sort(timeline.begin(), timeline.end(), [](const SendMessageTimelineKeyFrame* a, const SendMessageTimelineKeyFrame* b) {
        return a->time < b->time;  // 昇順にソート
    });
}

void SendMessageTimelineEditor::add_keyframe(float time) {
    // キーフレームを作成
    SendMessageTimelineKeyFrame* keyframe = new SendMessageTimelineKeyFrame{ time, "", "" };

    // データが無ければ作成
    if (data_ == nullptr) {
        std::vector<SendMessageTimelineKeyFrame*> timeline;
        data_ = new SendMessageTimelineParameter::SendMessageTimelineData(timeline);
    }

    vector<SendMessageTimelineKeyFrame*>& timeline = data_->get();
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
            const SendMessageTimelineKeyFrame* lhs,
            const SendMessageTimelineKeyFrame* rhs) {
                return lhs->time < rhs->time;
        }
    );
    timeline.insert(it, keyframe);
}

void SendMessageTimelineEditor::remove_keyframe(unsigned int index) {
    if (data_ == nullptr) return;
    vector<SendMessageTimelineKeyFrame*>& timeline = data_->get();

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

ordered_json SendMessageTimelineEditor::save_data() {
    ordered_json data;

    if (data_ == nullptr || data_->get().empty()) return data;

    for (const auto* kf : data_->get()) {
        ordered_json item;
        item["time"] = kf->time;
        item["target"] = kf->target;
        item["message"] = kf->message;
        data["timeline"].push_back(item);
    }
    return data;
}

void SendMessageTimelineEditor::load(const json& j) {
    clear();
    data_ = parameter_.create_data(j);
}

void SendMessageTimelineEditor::play() {
    if (data_ == nullptr) return;
    parameter_.play(data_);
}
