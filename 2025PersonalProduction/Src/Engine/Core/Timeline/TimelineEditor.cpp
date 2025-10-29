#include "TimelineEditor.h"
#include <imgui/imgui.h>
#include "Editor/ITimelineEditor.h"
#include "Engine/Utils/MyJson.h"
#include "Engine/Utils/Folder.h"

TimelineEditor::~TimelineEditor() {
    clear();
}

void TimelineEditor::update(float delta_time) {
    if (editors_.empty()) return;

    ImGui::Begin("Timeline Window");

    // ���C��GUI
    update_main_gui();

    // �L�[�t���[���̒ǉ�
    ImGui::Separator();
    update_add_keyframe();

    // �^�C�����C���r���[
    ImGui::Separator();
    update_timeline_view();

    ImGui::End();
}

void TimelineEditor::clear() {
    on_reset();
    for (auto& editor : editors_) {
        delete editor;
        editor = nullptr;
    }
    editors_.clear();
}

void TimelineEditor::add(ITimelineEditor* editor) {
    editors_.push_back(editor);
}

void TimelineEditor::update_main_gui() {
    // �Đ��{�^��
    if (ImGui::Button("Play##1")) on_play();
    // ���[�h�{�^��
    ImGui::SameLine();
    if (ImGui::Button("Load##1")) ImGui::OpenPopup("Load##2");
    if (ImGui::BeginPopupModal("Load##2", NULL, ImGuiWindowFlags_AlwaysAutoResize)) on_load();
    // �ۑ��{�^��
    ImGui::SameLine();
    if (ImGui::Button("Save##1")) on_save();
    //���Z�b�g�{�^��
    ImGui::SameLine();
    if (ImGui::Button("Reset##1")) on_reset();

    // �^�C�����C���f�[�^�̖��O���̓t�B�[���h
    ImGui::PushItemWidth(200);
    ImGui::InputText("Data Name##1", &name_);
    ImGui::PopItemWidth();
}

void TimelineEditor::update_add_keyframe() {
    ImGui::Text("KeyFrame");
    // �L�[�t���[���̒ǉ��{�^��
    if (ImGui::Button("Add##1")) {
        editors_[select_parameter_index_]->add_keyframe(add_keyframe_time_);
    }

    // �L�[�t���[���p�����[�^��I��
    ImGui::SameLine();
    // ���݂̃p�����[�^
    const std::string current_parameter_name = editors_[select_parameter_index_]->name();
    // �h���b�v�_�E�����X�g
    ImGui::PushItemWidth(160);
    if (ImGui::BeginCombo("Parameters##1", current_parameter_name.c_str())) {
        // �S�p�����[�^��I�����ɂ���
        for (unsigned int i = 0; i < editors_.size(); ++i) {
            bool is_selected = (select_parameter_index_ == i);
            // �I�����X�V
            if (ImGui::Selectable(editors_[select_parameter_index_]->name().c_str(), is_selected))  select_parameter_index_ = i;
            // �I���Ƀt�H�[�J�X
            if (is_selected)ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    // �L�[�t���[���̒ǉ����Ԃ����
    ImGui::SameLine();
    ImGui::PushItemWidth(80);
    ImGui::InputFloat("Time##1", &add_keyframe_time_);
    ImGui::PopItemWidth();

    // �p�����[�^�̏����l��ݒ�
    ImGui::Separator();
    ImGui::Text("Initial Parameter");
    editors_[select_parameter_index_]->update_keyframe_initial_parameters();
}

void TimelineEditor::update_timeline_view() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos;
    ImVec2 canvas_size;
    const float canvas_pos_x = ImGui::GetCursorScreenPos().x;

    const float MAX_TIME{ 10.0f };  // ��x�ɕ\�����鎞�Ԃ͈̔�
    const float TIMELINE_HEIGHT{ 20.0f };   // ����
    const unsigned int KEYFRAME_NOMOVE{ 999999 }; // �L�[�t���[���𓮂����Ă��Ȃ��Ƃ��̔ԍ�

    // ���Ԃ���X���W�֕ϊ�
    auto time_to_screen_x = [&](float absolute_time) {
        float relative_time = absolute_time - timeline_view_start_time_;
        return canvas_pos.x + (relative_time / MAX_TIME) * canvas_size.x;
    };

    // X���W���玞�Ԃ֕ϊ�
    auto screen_x_to_time = [&](float x) {
        float ratio = (x - canvas_pos.x) / canvas_size.x;
        return timeline_view_start_time_ + (ratio * MAX_TIME);
    };

    // �^�C�����C���r���[�̐擪���Ԃ̃X���C�_�[
    if (ImGui::SliderFloat(" ##10", &timeline_view_start_time_, 0.0, 100.0f)) {
        timeline_view_start_time_ = (int)timeline_view_start_time_;
    }

    // �b����`�悵�����ǂ���
    bool is_draw_time{ false };

    // �S�p�����[�^��`�悷��
    for (unsigned int i = 0; i < editors_.size(); ++i) {
        const auto& editor = editors_[i];
        // �L�[�t���[�����Ȃ��Ȃ�`�悵�Ȃ�
        if (editor->is_empty()) continue;

        // �ҏW�Ώۂ�I��
        if (ImGui::Button((editor->name() + "##1").c_str())) {
            edit_parameter_index_ = i;
            move_key_frame_index_ = KEYFRAME_NOMOVE;
        }
        ImGui::SameLine();
        canvas_pos = ImGui::GetCursorScreenPos();       // �`����n�߂���W(����)
        canvas_pos.x = canvas_pos_x + 80.0f;
        canvas_size = ImGui::GetContentRegionAvail();   // �`��͈�
        canvas_size.x -= 20.0f;
        canvas_size.y = TIMELINE_HEIGHT;

        // �w�i�̕`��
        draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), edit_parameter_index_ == i ? IM_COL32(50, 50, 50, 255) : IM_COL32(25, 25, 25, 255));

        // �ڐ���̕`��
        for (float t_offset = 0.0f; t_offset <= MAX_TIME; t_offset += 1.0f) {
            // ���݂̖ڐ���̐�Ύ��Ԃ��v�Z
            float absolute_tick_time = timeline_view_start_time_ + t_offset;
            // ��Ύ��Ԃ����W�ɕϊ�
            float x = time_to_screen_x(absolute_tick_time);
            // �`�揈��
            draw_list->AddLine(ImVec2(x, canvas_pos.y), ImVec2(x, canvas_pos.y + canvas_size.y), IM_COL32(100, 100, 100, 255));

            // ��ԏ�̃p�����[�^�����b����`�悷��
            if (!is_draw_time) {
                char time_str[16];
                snprintf(time_str, sizeof(time_str), "%.0fs", absolute_tick_time);
                draw_list->AddText(ImVec2(x + 3, canvas_pos.y), IM_COL32(200, 200, 200, 255), time_str);
            }
        }
        is_draw_time = true;

        // �L�[�t���[���̕`��
        unsigned int& edit_keyframe = editor->edit_keyframe();
        for (int j = 0; j < editor->count_keyframe(); ++j) {
            // �L�[�t���[���̍��W���擾
            const float key_time = editor->get_keyframe_time(j);
            const float marker_x = time_to_screen_x(key_time);
            ImVec2 marker_pos(marker_x, canvas_pos.y + TIMELINE_HEIGHT / 2.0f);

            // �͈͊O�Ȃ�X�L�b�v
            if (marker_pos.x < canvas_pos.x || marker_pos.x > canvas_pos.x + canvas_size.x) continue;

            // �L�[�t���[���`��
            const float marker_radius = 6.0f;
            ImU32 marker_color = (edit_parameter_index_ == i) && (j == edit_keyframe) ? IM_COL32(255, 255, 255, 255) : IM_COL32(25, 255, 255, 100);
            draw_list->AddCircleFilled(marker_pos, marker_radius, marker_color);

            // �ҏW���Ȃ�N���b�N���m
            if (edit_parameter_index_ != i) continue;
            // �}�E�X���L�[�t���[�����N���b�N�������ǂ���
            if (move_key_frame_index_ == KEYFRAME_NOMOVE && ImGui::IsMouseHoveringRect(
                ImVec2(marker_pos.x - marker_radius, marker_pos.y - marker_radius),
                ImVec2(marker_pos.x + marker_radius, marker_pos.y + marker_radius)
            )) {
                // �L�[�t���[�����h���b�O�ړ��ΏۂƂ���
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) edit_keyframe = move_key_frame_index_ = j;
            }
        }

        // �ҏW���Ȃ�h���b�O�ړ�
        if (edit_parameter_index_ == i && move_key_frame_index_ != KEYFRAME_NOMOVE) {
            // �}�E�X�ʒu����Đ����Ԃ��X�V
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
                editor->get_keyframe_time(move_key_frame_index_) = screen_x_to_time(ImGui::GetMousePos().x);
                if (editor->get_keyframe_time(move_key_frame_index_) < 0.0f) editor->get_keyframe_time(move_key_frame_index_) = 0.0f;
            }
            // �h���b�O���I��
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                editor->sort_timeline();
                move_key_frame_index_ = KEYFRAME_NOMOVE;
            }
        }
        // �ҏW���̃p�����[�^�`��͈͂̓}�E�X���͂𖳌�������
        if (edit_parameter_index_ == i) ImGui::InvisibleButton("timeline_canvas", canvas_size);
    }

    // �I�𒆂̃L�[�t���[���̃p�����[�^��ҏW
    ImGui::Separator();
    ImGui::Text("Edit Parameter");
    editors_[select_parameter_index_]->update_select_keyframe();
}

void TimelineEditor::on_play() {
    // ���݂̃f�[�^���Đ�������
    for (const auto& editor : editors_) {
        editor->play();
    }
}

void TimelineEditor::on_reset() {
    // ���݂̃f�[�^���폜������
    for (const auto& editor : editors_) {
        editor->clear();
    }
    name_ = "";
    select_parameter_index_ = 0;
    edit_parameter_index_ = 0;
    add_keyframe_time_ = 0.0f;
    timeline_view_start_time_ = 0.0f;
    move_key_frame_index_ = 999999;
}

void TimelineEditor::on_load() {
    ImGui::Text("please specify the continuation of Resource/Private/Behavior/Timeline/ (no extension)");

    ImGui::Separator();

    ImGui::InputText("file name##2", &load_file_name_);

    if (ImGui::Button("load##3", ImVec2(120, 0))) {
        // �ǂݍ���
        json j;
        if (!MyJson::is_json("Resource/Private/Behavior/Timeline/" + load_file_name_ + ".json", j)) return;

        // �Đ��p�̃f�[�^�����擾
        name_ = MyJson::get_string(j, "Name");
        // ��`����Ă���p�����[�^��S�ĉ�
        for (auto it = j.begin(); it != j.end(); ++it) {
            const std::string editor_name = it.key();
            ITimelineEditor* editor = find(editor_name);
            if (editor == nullptr) continue;

            // �Ή�����p�����[�^�ɃI�u�W�F�N�g��n��
            editor->load(*it);
        }

        ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
    }
    ImGui::SameLine();
    if (ImGui::Button("cancel##2", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
    }
    ImGui::EndPopup();
}

void TimelineEditor::on_save() {
    ordered_json j;
    // ���O��ݒ�
    j["Name"] = name_;
    for (const auto& editor : editors_) {
        // �O�̂��߃\�[�g
        editor->sort_timeline();
        // �Z�[�u�f�[�^���擾���i�[
        j[editor->name()] = editor->save_data();
    }
    
    // �t�@�C���p�X
    const std::string root{ "Resource/Private/Behavior/Timeline" };
    // �㏑���ۑ�
    MyLib::create_folder(root);
    MyLib::write_to_file(root + "/" + name_ + ".json", j);
}

ITimelineEditor* TimelineEditor::find(const std::string& name) {
    for (auto& editor : editors_) {
        if (editor->name() == name) return editor;
    }
    return nullptr;
}
