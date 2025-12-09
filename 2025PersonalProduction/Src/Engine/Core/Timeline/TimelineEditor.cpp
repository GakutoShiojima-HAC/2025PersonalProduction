#include "TimelineEditor.h"
#include <imgui/imgui.h>
#include "Editor/ITimelineEditor.h"
#include "Engine/Utils/MyJson.h"
#include "Engine/Utils/Folder.h"
#include "Engine/Utils/MyString.h"

TimelineEditor::~TimelineEditor() {
    clear();
}

void TimelineEditor::update(float delta_time) {
    if (editors_.empty()) return;

    ImGui::Begin("Timeline Window");

    // メインGUI
    update_main_gui();

    // キーフレームの追加
    ImGui::Separator();
    update_add_keyframe();

    // タイムラインビュー
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
    // 再生ボタン
    if (ImGui::Button(ToUTF8("再生").c_str())) on_play();
    // ロードボタン
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("読み込み").c_str())) ImGui::OpenPopup("Load##2");
    if (ImGui::BeginPopupModal("Load##2", NULL, ImGuiWindowFlags_AlwaysAutoResize)) on_load();
    // 保存ボタン
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("保存").c_str())) on_save();
    //リセットボタン
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("リセット").c_str())) on_reset();

    // タイムラインデータの名前入力フィールド
    ImGui::PushItemWidth(200);
    ImGui::InputText(ToUTF8("名前").c_str(), &name_);
    ImGui::PopItemWidth();
}

void TimelineEditor::update_add_keyframe() {
    // キーフレームの追加ボタン
    if (ImGui::Button(ToUTF8("キーフレームを追加").c_str())) {
        editors_[select_parameter_index_]->add_keyframe(add_keyframe_time_);
    }

    // キーフレームパラメータを選ぶ
    ImGui::SameLine();
    // 現在のパラメータ
    const std::string current_parameter_name = editors_[select_parameter_index_]->name();
    // ドロップダウンリスト
    ImGui::PushItemWidth(160);
    if (ImGui::BeginCombo(ToUTF8("パラメータを選択").c_str(), current_parameter_name.c_str())) {
        // 全パラメータを選択肢にする
        for (unsigned int i = 0; i < editors_.size(); ++i) {
            bool is_selected = (select_parameter_index_ == i);
            // 選択を更新
            if (ImGui::Selectable(editors_[i]->name().c_str(), is_selected))  select_parameter_index_ = i;
            // 選択にフォーカス
            if (is_selected)ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    // キーフレームの追加時間を入力
    ImGui::SameLine();
    ImGui::PushItemWidth(80);
    ImGui::InputFloat(ToUTF8("キーフレームを追加する時間").c_str(), &add_keyframe_time_);
    ImGui::PopItemWidth();

    // パラメータの初期値を設定
    ImGui::Separator();
    ImGui::Text(ToUTF8("キーフレームの初期値").c_str());
    editors_[select_parameter_index_]->update_keyframe_initial_parameters();
}

void TimelineEditor::update_timeline_view() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos;
    ImVec2 canvas_size;
    const float canvas_pos_x = ImGui::GetCursorScreenPos().x;

    const float MAX_TIME{ 10.0f };  // 一度に表示する時間の範囲
    const float TIMELINE_HEIGHT{ 20.0f };   // 高さ
    const unsigned int KEYFRAME_NOMOVE{ 999999 }; // キーフレームを動かしていないときの番号

    // 時間からX座標へ変換
    auto time_to_screen_x = [&](float absolute_time) {
        float relative_time = absolute_time - timeline_view_start_time_;
        return canvas_pos.x + (relative_time / MAX_TIME) * canvas_size.x;
    };

    // X座標から時間へ変換
    auto screen_x_to_time = [&](float x) {
        float ratio = (x - canvas_pos.x) / canvas_size.x;
        return timeline_view_start_time_ + (ratio * MAX_TIME);
    };

    // タイムラインビューの先頭時間のスライダー
    if (ImGui::SliderFloat(" ##10", &timeline_view_start_time_, 0.0, 100.0f)) {
        timeline_view_start_time_ = (int)timeline_view_start_time_;
    }

    // 秒数を描画したかどうか
    bool is_draw_time{ false };

    // 全パラメータを描画する
    for (unsigned int i = 0; i < editors_.size(); ++i) {
        const auto& editor = editors_[i];
        // キーフレームがないなら描画しない
        if (editor->is_empty()) continue;

        canvas_pos = ImGui::GetCursorScreenPos();       // 描画を始める座標(左上)
        canvas_pos.x = canvas_pos_x + 100.0f;
        canvas_size = ImGui::GetContentRegionAvail();   // 描画範囲
        canvas_size.x -= 20.0f;
        canvas_size.y = TIMELINE_HEIGHT;

        // 編集対象を選択
        if (ImGui::Button((editor->name() + "##1").c_str())) {
            edit_parameter_index_ = i;
            move_key_frame_index_ = KEYFRAME_NOMOVE;
        }
        ImGui::SameLine();

        // 背景の描画
        draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), edit_parameter_index_ == i ? IM_COL32(50, 50, 50, 255) : IM_COL32(25, 25, 25, 255));

        // 目盛りの描画
        for (float t_offset = 0.0f; t_offset <= MAX_TIME; t_offset += 1.0f) {
            // 現在の目盛りの絶対時間を計算
            float absolute_tick_time = timeline_view_start_time_ + t_offset;
            // 絶対時間を座標に変換
            float x = time_to_screen_x(absolute_tick_time);
            // 描画処理
            draw_list->AddLine(ImVec2(x, canvas_pos.y), ImVec2(x, canvas_pos.y + canvas_size.y), IM_COL32(100, 100, 100, 255));

            // 一番上のパラメータだけ秒数を描画する
            if (!is_draw_time) {
                char time_str[16];
                snprintf(time_str, sizeof(time_str), "%.0fs", absolute_tick_time);
                draw_list->AddText(ImVec2(x + 3, canvas_pos.y), IM_COL32(200, 200, 200, 255), time_str);
            }
        }
        is_draw_time = true;

        // キーフレームの描画
        unsigned int& edit_keyframe = editor->edit_keyframe();
        for (int j = 0; j < editor->count_keyframe(); ++j) {
            // キーフレームの座標を取得
            const float key_time = editor->get_keyframe_time(j);
            const float marker_x = time_to_screen_x(key_time);
            ImVec2 marker_pos(marker_x, canvas_pos.y + TIMELINE_HEIGHT / 2.0f);

            // 範囲外ならスキップ
            if (marker_pos.x < canvas_pos.x || marker_pos.x > canvas_pos.x + canvas_size.x) continue;

            // キーフレーム描画
            const float marker_radius = 6.0f;
            ImU32 marker_color = (edit_parameter_index_ == i) && (j == edit_keyframe) ? IM_COL32(255, 255, 255, 255) : IM_COL32(25, 255, 255, 100);
            draw_list->AddCircleFilled(marker_pos, marker_radius, marker_color);

            // 編集中ならクリック検知
            if (edit_parameter_index_ != i) continue;
            // マウスがキーフレームをクリックしたかどうか
            if (move_key_frame_index_ == KEYFRAME_NOMOVE && ImGui::IsMouseHoveringRect(
                ImVec2(marker_pos.x - marker_radius, marker_pos.y - marker_radius),
                ImVec2(marker_pos.x + marker_radius, marker_pos.y + marker_radius)
            )) {
                // キーフレームをドラッグ移動対象とする
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) edit_keyframe = move_key_frame_index_ = j;
            }
        }
        
        // 編集中ならドラッグ移動
        if (edit_parameter_index_ == i && move_key_frame_index_ != KEYFRAME_NOMOVE) {
            /*
            // マウス位置から再生時間を更新
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
                editor->get_keyframe_time(move_key_frame_index_) = screen_x_to_time(ImGui::GetMousePos().x);
                if (editor->get_keyframe_time(move_key_frame_index_) < 0.0f) editor->get_keyframe_time(move_key_frame_index_) = 0.0f;
            }
            */
            // ドラッグを終了
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                editor->sort_timeline();
                move_key_frame_index_ = KEYFRAME_NOMOVE;
            }
        }
        // 編集中のパラメータ描画範囲はマウス入力を無効化する
        if (edit_parameter_index_ == i) ImGui::InvisibleButton("timeline_canvas", canvas_size);

        ImGui::Separator();
    }

    // 選択中のキーフレームのパラメータを編集
    ImGui::Separator();
    ImGui::Text(ToUTF8("キーフレームの編集").c_str());
    editors_[edit_parameter_index_]->update_select_keyframe();
}

void TimelineEditor::on_play() {
    // 現在のデータを再生させる
    for (const auto& editor : editors_) {
        editor->play();
    }
}

void TimelineEditor::on_reset() {
    // 現在のデータを削除させる
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
    ImGui::Text(ToUTF8("Resource/Private/Behavior/Timeline/ にあるファイル名を入力してください。（拡張子なし）").c_str());

    ImGui::Separator();

    ImGui::InputText(ToUTF8("ファイル名").c_str(), &load_file_name_);

    if (ImGui::Button(ToUTF8("読み込み").c_str(), ImVec2(120, 0))) {
        // 読み込む
        json j;
        if (!MyJson::is_json("Resource/Private/Behavior/Timeline/" + load_file_name_ + ".json", j)) {
            ImGui::CloseCurrentPopup(); // ポップアップを閉じる
        }

        // 再生用のデータ名を取得
        name_ = MyJson::get_string(j, "Name");
        // 定義されているパラメータを全て回す
        for (auto it = j.begin(); it != j.end(); ++it) {
            const std::string editor_name = it.key();
            ITimelineEditor* editor = find(editor_name);
            if (editor == nullptr) continue;

            // 対応するパラメータにオブジェクトを渡す
            editor->load(*it);
        }

        ImGui::CloseCurrentPopup(); // ポップアップを閉じる
    }
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("キャンセル").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup(); // ポップアップを閉じる
    }
    ImGui::EndPopup();
}

void TimelineEditor::on_save() {
    ordered_json j;
    // 名前を設定
    j["Name"] = name_;
    for (const auto& editor : editors_) {
        // 念のためソート
        editor->sort_timeline();
        // セーブデータを取得し格納
        j[editor->name()] = editor->save_data();
    }
    
    // ファイルパス
    const std::string root{ "Resource/Private/Behavior/Timeline" };
    // 上書き保存
    MyLib::create_folder(root);
    MyLib::write_to_file(root + "/" + name_ + ".json", j);
}

ITimelineEditor* TimelineEditor::find(const std::string& name) {
    for (auto& editor : editors_) {
        if (editor->name() == name) return editor;
    }
    return nullptr;
}
