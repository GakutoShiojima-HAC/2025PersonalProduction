#include "Engine/Core/Timeline/Editor/TimelineEditor.h"

void TimelineEditor::set_keyframe(int value, int vec_size) {
    int tmp = (int)current_edit_keyframe_ + value;
    if (tmp <= 0) tmp = 0;   // clamp���ƃ��[�v���Ă��܂��̂�
    else if (tmp >= vec_size) tmp = vec_size - 1;
    current_edit_keyframe_ = (GSuint)tmp;
}

void TimelineEditor::draw_move_keyframe(int max_size) {
    if (ImGui::Button("<")) set_keyframe(-1, max_size);
    ImGui::SameLine();
    ImGui::Text("%d", (int)current_edit_keyframe_);
    ImGui::SameLine();
    if (ImGui::Button(">")) set_keyframe(1, max_size);
    ImGui::SameLine();
    ImGui::Text("current keyframe. range is 0 to %d", max_size - 1);
}

void TimelineEditor::draw_main_gui(bool timeline_is_empty, const string& file_path) {
    draw_play();
    ImGui::SameLine();
    draw_load(file_path);
    ImGui::SameLine();
    draw_save(timeline_is_empty, file_path);
    ImGui::SameLine();
    draw_reset();
}

void TimelineEditor::draw_play() {
    if (ImGui::Button("Play##1")) play();
}

void TimelineEditor::draw_load(const string& file_path) {
    if (ImGui::Button("Load##1")) ImGui::OpenPopup("Load##2");

    if (ImGui::BeginPopupModal("Load##2", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(("please specify the continuation of" + file_path +  " (no extension)").c_str());

        ImGui::Separator();

        ImGui::InputText("file name##2", &load_filename_);

        if (ImGui::Button("load##3", ImVec2(120, 0))) {
            load(); // �t�@�C���ǂݍ���
            ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
        }
        ImGui::SameLine();
        if (ImGui::Button("cancel##2", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
        }
        ImGui::EndPopup();
    }
}

void TimelineEditor::draw_save(bool timeline_is_empty, const string& file_path) {
    if (ImGui::Button("Save##1")) {
        if (timeline_is_empty) ImGui::OpenPopup("Error");   // error
        else ImGui::OpenPopup("Save File");
    }

    if (ImGui::BeginPopupModal("Save File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(("output to" + file_path + " in json format.").c_str());

        ImGui::Separator();

        ImGui::InputText("file name##1", &save_filename_);

        if (ImGui::Button("save##2", ImVec2(120, 0))) {
            // �t�@�C���ۑ�
            save();
            ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
        }
        ImGui::SameLine();
        if (ImGui::Button("cancel##1", ImVec2(120, 0))) ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("keyframe does not exist");
        ImGui::Text("requires at least one keyframe.");

        ImGui::Separator();

        if (ImGui::Button("OK##1", ImVec2(120, 0))) ImGui::CloseCurrentPopup(); // �|�b�v�A�b�v�����
        ImGui::EndPopup();
    }
}

void TimelineEditor::draw_reset() {
    if (ImGui::Button("Reset##1")) reset();
}

void TimelineEditor::draw_timeline(bool timeline_is_empty, int max_size) {
    if (timeline_is_empty) return;

    // �L�[�t���[���̈ړ�
    ImGui::Separator();
    draw_move_keyframe(max_size);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();        // �`��J�n�̃X�N���[�����W
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    canvas_size.y = TIMELINE_HEIGHT;

    // �w�i
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255));

    // ���Ԃ���X���W�֕ϊ�
    auto time_to_screen_x = [&](float absolute_time) {
        float relative_time = absolute_time - timeline_start_time_;
        return canvas_pos.x + (relative_time / TIMELINE_MAX_TIME) * canvas_size.x;
    };

    // X���W���玞�Ԃ֕ϊ�
    auto screen_x_to_time = [&](float x) {
        float ratio = (x - canvas_pos.x) / canvas_size.x;
        return timeline_start_time_ + (ratio * TIMELINE_MAX_TIME);
    };

    // �ڐ���`��
    for (float t_offset = 0.0f; t_offset <= TIMELINE_MAX_TIME; t_offset += 1.0f) {
        // ���݂̖ڐ���̐�Ύ��Ԃ��v�Z
        float absolute_tick_time = timeline_start_time_ + t_offset;
        // �ύX���timeToScreenX�ɐ�Ύ��Ԃ�n��
        float x = time_to_screen_x(absolute_tick_time);

        // �`�揈��
        draw_list->AddLine(ImVec2(x, canvas_pos.y), ImVec2(x, canvas_pos.y + TIMELINE_HEIGHT), IM_COL32(100, 100, 100, 255));
        char time_str[16];
        snprintf(time_str, sizeof(time_str), "%.0fs", absolute_tick_time);
        draw_list->AddText(ImVec2(x + 3, canvas_pos.y), IM_COL32(200, 200, 200, 255), time_str);
    }

    for (int i = 0; i < max_size; ++i) {
        // ���������ɓ����Ă��邩�ǂ���
        float key_time = get_time(i);
        //if (key_time < timeline_start_time_ || key_time > TIMELINE_MAX_TIME) continue;

        // �L�[�t���[���̍��W���擾
        float marker_x = time_to_screen_x(key_time);
        ImVec2 marker_pos(marker_x, canvas_pos.y + TIMELINE_HEIGHT / 2.0f);

        // �L�[�t���[���`��
        const float marker_radius = 6.0f;
        ImU32 marker_color = (i == current_edit_keyframe_) ? IM_COL32(255, 255, 255, 255) : IM_COL32(25, 255, 255, 100);
        draw_list->AddCircleFilled(marker_pos, marker_radius, marker_color);

        // �}�E�X���L�[�t���[�����N���b�N�������ǂ���
        if (current_move_keyframe_ == NOMOVE && ImGui::IsMouseHoveringRect(
            ImVec2(marker_pos.x - marker_radius, marker_pos.y - marker_radius),
            ImVec2(marker_pos.x + marker_radius, marker_pos.y + marker_radius)
        )) {
            // �L�[�t���[�����h���b�O�ړ��ΏۂƂ���
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) current_edit_keyframe_ = current_move_keyframe_ = i;
        }
    }

    // �L�[�t���[���̃h���b�O�ړ�
    if (current_move_keyframe_ != NOMOVE) {
        // �}�E�X�ʒu����Đ����Ԃ��X�V
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
            get_time(current_move_keyframe_) = screen_x_to_time(ImGui::GetMousePos().x);
            if (get_time(current_move_keyframe_) < 0.0f) get_time(current_move_keyframe_) = 0.0f;
        }

        // �h���b�O���I��
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            sort_timeline();
            current_move_keyframe_ = NOMOVE;
        }
    }

    // �Đ��w�b�h(�Ԃ����)
    float head_x = time_to_screen_x(time_);
    draw_list->AddLine(ImVec2(head_x, canvas_pos.y), ImVec2(head_x, canvas_pos.y + TIMELINE_HEIGHT), IM_COL32(255, 0, 0, 255), 2.0f);

    // �}�E�X����
    ImGui::InvisibleButton("timeline_canvas", canvas_size);
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        // �h���b�O�ōĐ��w�b�h���ړ�
        time_ = screen_x_to_time(ImGui::GetMousePos().x);
        // �N�����v
        const float timeline_end_time = timeline_start_time_ + TIMELINE_MAX_TIME;
        if (time_ < timeline_start_time_) time_ = timeline_start_time_;
        if (time_ > timeline_end_time) time_ = timeline_end_time;
    }
    // �N���b�N�ł��ړ�
    if (ImGui::IsItemClicked()) {
        time_ = screen_x_to_time(ImGui::GetMousePos().x);
    }

    // �^�C�����C���X���C�_�[
    ImGui::PushItemWidth(-1.0f);
    if (ImGui::SliderFloat(" ##10", &timeline_start_time_, 0.0, get_time(max_size - 1))) {
        timeline_start_time_ = (int)timeline_start_time_;
    }
    ImGui::PopItemWidth();
}
