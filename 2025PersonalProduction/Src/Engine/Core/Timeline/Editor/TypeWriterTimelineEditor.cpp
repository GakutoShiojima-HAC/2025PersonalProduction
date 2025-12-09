#include "TypeWriterTimelineEditor.h"
#include "Engine/Utils/MyString.h"

// 参照返しのエラー回避用
static float EMPTY_TIME{ 0.0f };

TypeWriterTimelineEditor::TypeWriterTimelineEditor(TypeWriterTimelineParameter& parameter) :
    parameter_{ parameter } {

}

TypeWriterTimelineEditor::~TypeWriterTimelineEditor() {
    clear();
}

void TypeWriterTimelineEditor::clear() {
    if (data_ != nullptr) data_->clear();
    delete data_;
    data_ = nullptr;
    edit_keyframe_index_ = 0;
}

void TypeWriterTimelineEditor::update_select_keyframe() {
    if (data_ == nullptr) return;
    vector<TypeWriterTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;
    TypeWriterTimelineKeyFrame* key_frame = timeline[edit_keyframe_index_];
    if (key_frame == nullptr) return;

    // 時間を編集
    ImGui::PushItemWidth(80);
    if (ImGui::InputFloat(ToUTF8("キーフレームの時間").c_str(), &key_frame->time)) {
        sort_timeline();
        auto it = find(timeline.begin(), timeline.end(), key_frame);
        if (it != timeline.end()) edit_keyframe_index_ = distance(timeline.begin(), it);
    }
    ImGui::PopItemWidth();

    for (int i = 0; i < key_frame->text.size(); i++)
    {
        ImGui::PushID(i);

        char buf[256];
        std::snprintf(buf, sizeof(buf), "%s", key_frame->text[i].c_str());

        if (ImGui::InputText("text##1", buf, sizeof(buf))) {
            key_frame->text[i] = buf;
        }

        ImGui::SameLine();
        if (ImGui::Button(ToUTF8("この行を削除").c_str())) {
            key_frame->text.erase(key_frame->text.begin() + i);
            ImGui::PopID();
            break;
        }

        ImGui::PopID();
    }

    if (ImGui::Button(ToUTF8("テキストを追加").c_str())) {
        key_frame->text.push_back("");
    }

    // キーフレームを削除
    if (ImGui::Button(ToUTF8("選択中のキーフレームを削除").c_str())) remove_keyframe(edit_keyframe_index_);
}

std::string TypeWriterTimelineEditor::name() const {
    return parameter_.name();
}

bool TypeWriterTimelineEditor::is_empty() const {
    if (data_ == nullptr) return true;
    return data_->get().empty();
}

unsigned int TypeWriterTimelineEditor::count_keyframe() const {
    if (data_ == nullptr) return 0;
    return data_->get().size();
}

float& TypeWriterTimelineEditor::get_keyframe_time(unsigned int index) {
    if (data_ == nullptr) return EMPTY_TIME;
    std::vector<TypeWriterTimelineKeyFrame*>& data = data_->get();
    if (data.empty() || index >= data.size()) return EMPTY_TIME;
    return data[index]->time;
}

void TypeWriterTimelineEditor::sort_timeline() {
    if (data_ == nullptr) return;
    vector<TypeWriterTimelineKeyFrame*>& timeline = data_->get();
    if (timeline.empty()) return;

    sort(timeline.begin(), timeline.end(), [](const TypeWriterTimelineKeyFrame* a, const TypeWriterTimelineKeyFrame* b) {
        return a->time < b->time;  // 昇順にソート
    });
}

void TypeWriterTimelineEditor::add_keyframe(float time) {
    // キーフレームを作成
    TypeWriterTimelineKeyFrame* keyframe = new TypeWriterTimelineKeyFrame{ time, std::vector<std::string>{} };

    // データが無ければ作成
    if (data_ == nullptr) {
        std::vector<TypeWriterTimelineKeyFrame*> timeline;
        data_ = new TypeWriterTimelineParameter::TypeWriterTimelineData(timeline);
    }

    vector<TypeWriterTimelineKeyFrame*>& timeline = data_->get();
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
            const TypeWriterTimelineKeyFrame* lhs,
            const TypeWriterTimelineKeyFrame* rhs) {
                return lhs->time < rhs->time;
        }
    );
    timeline.insert(it, keyframe);
}

void TypeWriterTimelineEditor::remove_keyframe(unsigned int index) {
    if (data_ == nullptr) return;
    vector<TypeWriterTimelineKeyFrame*>& timeline = data_->get();

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

ordered_json TypeWriterTimelineEditor::save_data() {
    ordered_json data;

    if (data_ == nullptr || data_->get().empty()) return data;

    for (const auto* kf : data_->get()) {
        ordered_json item;
        item["time"] = kf->time;
        item["text"] = kf->text;
        data["timeline"].push_back(item);
    }
    return data;
}

void TypeWriterTimelineEditor::load(const json& j) {
    clear();
    data_ = parameter_.create_data(j);
}

void TypeWriterTimelineEditor::play() {
    if (data_ == nullptr) return;
    parameter_.play(data_);
}
