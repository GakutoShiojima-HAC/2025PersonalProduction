#include "TimelineEditor.h"
#include <imgui/imgui.h>
#include "Editor/ITimelineEditor.h"
#include "Engine/Utils/MyJson.h"
#include "Engine/Utils/Folder.h"
#include "Engine/Utils/MyString.h"

const ImVec2 IM_BUTTON_SIZE{ 100.0f, 20.0f };
const ImVec2 IM_BUTTON_BIG_SIZE{ 200.0f, 20.0f };

TimelineEditor::~TimelineEditor() {
    clear();
}

void TimelineEditor::update(float delta_time) {
    if (editors_.empty()) return;

    ImGui::Begin("Timeline Editor Window");

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
    if (ImGui::Button(ToUTF8("再生").c_str(), IM_BUTTON_SIZE)) on_play();
    // ロードボタン
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("読み込み").c_str(), IM_BUTTON_SIZE)) ImGui::OpenPopup("Load##2");
    if (ImGui::BeginPopupModal("Load##2", NULL, ImGuiWindowFlags_AlwaysAutoResize)) on_load();
    // 保存ボタン
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("保存").c_str(), IM_BUTTON_SIZE)) on_save();
    //リセットボタン
    ImGui::SameLine();
    if (ImGui::Button(ToUTF8("リセット").c_str(), IM_BUTTON_SIZE)) on_reset();

    // タイムラインデータの名前入力フィールド
    ImGui::PushItemWidth(200);
    ImGui::InputText(ToUTF8("データ・ファイル名").c_str(), &name_);
    ImGui::PopItemWidth();
}

void TimelineEditor::update_add_keyframe() {
    // キーフレームの追加ボタン
    if (ImGui::Button(ToUTF8("キーフレームを追加").c_str(), IM_BUTTON_BIG_SIZE)) {
        editors_[select_parameter_index_]->add_keyframe(add_keyframe_time_);
    }

    // キーフレームパラメータを選ぶ
    ImGui::SameLine();
    // 現在のパラメータ
    const std::string current_parameter_name = editors_[select_parameter_index_]->name();
    // ドロップダウンリスト
    ImGui::PushItemWidth(200);
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
    ImGui::PushItemWidth(100);
    ImGui::InputFloat(ToUTF8("キーフレームを追加する時間").c_str(), &add_keyframe_time_);
    ImGui::PopItemWidth();

    // パラメータの初期値を設定
    ImGui::Separator();
    ImGui::Text(ToUTF8("キーフレームの初期値").c_str());
    editors_[select_parameter_index_]->update_keyframe_initial_parameters();
}

void TimelineEditor::update_timeline_view() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // パラメータラベルの幅
    const float LABEL_WIDTH{ 120.0f };
    // 行の高さ
    const float ROW_HEIGHT{ 25.0f };
    // 一度に表示する時間の範囲
    const float MAX_TIME{ 10.0f };
    // キーフレームの半径
    const float KF_RADIUS{ 7.0f };

    // タイムラインビューの先頭時間のスライダー
    if (ImGui::SliderFloat(ToUTF8("トラックの表示時間を変更").c_str(), &timeline_view_start_time_, 0.0, 100.0f)) {
        timeline_view_start_time_ = (int)timeline_view_start_time_;
    }

    // ヘッダーのサイズ
    const ImVec2 HEADER_POS = ImGui::GetCursorScreenPos();
    const float CANVAS_WIDTH = ImGui::GetContentRegionAvail().x - LABEL_WIDTH - 20.0f;

    // ヘッダーの描画
    for (float t_offset = 0.0f; t_offset <= MAX_TIME; t_offset += 1.0f) {
        const float abs_t = timeline_view_start_time_ + t_offset;
        // ヘッダー位置基準のX座標計算
        const float x = HEADER_POS.x + LABEL_WIDTH + (t_offset / MAX_TIME) * CANVAS_WIDTH;

        char time_str[16];
        snprintf(time_str, sizeof(time_str), "%.0fs", abs_t);
        draw_list->AddText(ImVec2(x, HEADER_POS.y), IM_COL32(200, 200, 200, 255), time_str);
    }
    ImGui::Dummy(ImVec2(0, 20)); // 時間表示用のスペース確保
    ImGui::Separator();

    // パラメータの描画開始位置
    std::vector<ImVec2> row_start_positions;
    row_start_positions.reserve(editors_.size());

    // トラック
    for (unsigned int i = 0; i < editors_.size(); ++i) {
        auto& editor = editors_[i];

        const ImVec2 current_row_pos = ImGui::GetCursorScreenPos(); // 現在の行の左上座標
        row_start_positions.push_back(current_row_pos);             // 座標を保存

        ImGui::BeginGroup();
        // 選択中のパラメータラベル
        if (edit_parameter_index_ == i) {
            ImGui::Text("%s", editor->name().c_str());
        }
        // 非選択中のパラメータラベル
        else {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.65f), "%s", editor->name().c_str());
        }
        ImGui::EndGroup();

        // パラメータの基本情報
        ImGui::SameLine(LABEL_WIDTH);
        const ImVec2 timeline_start = ImGui::GetCursorScreenPos();
        const ImVec2 timeline_size(CANVAS_WIDTH, ROW_HEIGHT);

        // 背景
        const ImU32 bg_col = (edit_parameter_index_ == i) ? IM_COL32(50, 50, 50, 255) : IM_COL32(30, 30, 30, 255);
        draw_list->AddRectFilled(timeline_start, ImVec2(timeline_start.x + CANVAS_WIDTH, timeline_start.y + ROW_HEIGHT), bg_col);

        // 縦線
        for (float t_offset = 0.0f; t_offset <= MAX_TIME; t_offset += 1.0f) {
            float x = timeline_start.x + (t_offset / MAX_TIME) * CANVAS_WIDTH;
            draw_list->AddLine(ImVec2(x, timeline_start.y), ImVec2(x, timeline_start.y + ROW_HEIGHT), IM_COL32(60, 60, 60, 255));
        }

        // エディタの再生時間を取得
        const float current_play_time = editor->play_time();
        // 再生ヘッドの描画
        if (current_play_time >= 0.0f) {
            // 表示範囲の時間に変換
            const float relative_play_time = current_play_time - timeline_view_start_time_;
            // 表示範囲に入っているか
            if (relative_play_time >= 0.0f && relative_play_time <= MAX_TIME) {
                // 時間からX座標を計算
                const float head_x = timeline_start.x + (relative_play_time / MAX_TIME) * CANVAS_WIDTH;

                draw_list->AddLine(
                    ImVec2(head_x, timeline_start.y), ImVec2(head_x, timeline_start.y + ROW_HEIGHT),
                    IM_COL32(255, 25, 25, 255), 1.5f
                );
            }
        }

        // トラックをクリックでパラメータを選択
        ImGui::PushID(i);
        if (ImGui::InvisibleButton("RowTrigger", timeline_size)) {
            // 念のため、ドラッグ移動中なら保存
            if (drag_key_frame_index_ != NO_DRAG) {
                editors_[edit_parameter_index_]->sort_timeline();
                drag_key_frame_index_ = NO_DRAG;
            }

            edit_parameter_index_ = i;
        }
        ImGui::PopID();

        // 次の行へ
        ImGui::SetCursorScreenPos(ImVec2(current_row_pos.x, current_row_pos.y + ROW_HEIGHT));
        ImGui::Separator();
    }

    // レイアウト終了位置を保存
    ImVec2 layout_end_pos = ImGui::GetCursorScreenPos();

    // キーフレーム
    for (unsigned int i = 0; i < editors_.size(); ++i) {
        auto& editor = editors_[i];

        // 保存しておいた座標から座標を取得
        const ImVec2 row_pos = row_start_positions[i];
        const ImVec2 timeline_start = ImVec2(row_pos.x + LABEL_WIDTH, row_pos.y);

        for (int j = 0; j < editor->count_keyframe(); ++j) {
            const float key_time = editor->get_keyframe_time(j);
            const float relative_time = key_time - timeline_view_start_time_;
            // 表示範囲外なら無視
            if (relative_time < 0.0f || relative_time > MAX_TIME) continue;

            // 座標計算
            const float kf_x = timeline_start.x + (relative_time / MAX_TIME) * CANVAS_WIDTH;
            const ImVec2 kf_pos(kf_x, timeline_start.y + ROW_HEIGHT / 2.0f);

            // キーフレームを選択中
            if (ImGui::IsMouseHoveringRect(
                ImVec2(kf_pos.x - KF_RADIUS, kf_pos.y - KF_RADIUS),
                ImVec2(kf_pos.x + KF_RADIUS, kf_pos.y + KF_RADIUS)
            ) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                // 未選択のキーフレームを選択
                if (editor->edit_keyframe() != j) {
                    editor->edit_keyframe() = j;
                    edit_parameter_index_ = i;          // 行も選択状態に
                    drag_key_frame_index_ = NO_DRAG;    // 念のため既存の選択を解除
                }
                // キーフレームを再選択
                else if (edit_parameter_index_ == i && editor->edit_keyframe() == j) {
                    drag_key_frame_index_ = j;  // 選択中にする
                }
            }

            // 選択したキーフレームのドラッグ移動
            if (edit_parameter_index_ == i && drag_key_frame_index_ != NO_DRAG) {
                // ドラッグでキーフレームを移動
                if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
                    // マウス座標から時間を計算
                    const float mouse_x = ImGui::GetMousePos().x;
                    const float offset_x = mouse_x - timeline_start.x;
                    const float ratio = offset_x / CANVAS_WIDTH;
                    float current_time = timeline_view_start_time_ + (ratio * MAX_TIME);
                    if (current_time < 0.0f) current_time = 0.0f;

                    // キーフレームに適用
                    editor->get_keyframe_time(drag_key_frame_index_) = current_time;
                }
            }
            // 選択したキーフレームのドラッグ移動を終了
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && drag_key_frame_index_ != NO_DRAG) {
                editor->sort_timeline();
                drag_key_frame_index_ = NO_DRAG;
            }
        }
    }

    // ドラッグ
    if (drag_key_frame_index_ != NO_DRAG) {
        auto& current_editor = editors_[edit_parameter_index_];
        // 移動
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
            // 現在の座標を取得
            const ImVec2 row_pos = row_start_positions[edit_parameter_index_];
            const ImVec2 timeline_start = ImVec2(row_pos.x + LABEL_WIDTH, row_pos.y);
            // マウス座標から時間を計算
            const float mouse_x = ImGui::GetMousePos().x;
            const float offset_x = mouse_x - timeline_start.x;
            const float ratio = offset_x / CANVAS_WIDTH;
            float current_time = timeline_view_start_time_ + (ratio * MAX_TIME);
            if (current_time < 0.0f) current_time = 0.0f;

            // キーフレームに適用
            current_editor->get_keyframe_time(drag_key_frame_index_) = current_time;
        }
        // 終了
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            current_editor->sort_timeline();
            drag_key_frame_index_ = NO_DRAG;
        }
    }

    // キーフレームの描画
    for (unsigned int i = 0; i < editors_.size(); ++i) {
        auto& editor = editors_[i];

        // 保存しておいた座標から座標を取得
        const ImVec2 row_pos = row_start_positions[i];
        const ImVec2 timeline_start = ImVec2(row_pos.x + LABEL_WIDTH, row_pos.y);

        for (int j = 0; j < editor->count_keyframe(); ++j) {
            const float key_time = editor->get_keyframe_time(j);
            const float relative_time = key_time - timeline_view_start_time_;
            // 表示範囲外なら無視
            if (relative_time < 0.0f || relative_time > MAX_TIME) continue;

            // 座標計算
            const float kf_x = timeline_start.x + (relative_time / MAX_TIME) * CANVAS_WIDTH;
            const ImVec2 kf_pos(kf_x, timeline_start.y + ROW_HEIGHT / 2.0f);

            // キーフレームの描画
            const bool is_selected = (edit_parameter_index_ == i && editor->edit_keyframe() == j);
            const ImU32 kf_col = is_selected ? IM_COL32(255, 255, 255, 255) : IM_COL32(0, 200, 200, 180);
            draw_list->AddCircleFilled(kf_pos, KF_RADIUS, kf_col);
        }
    }

    // カーソル位置を戻す
    ImGui::SetCursorScreenPos(layout_end_pos);

    // 選択中のキーフレームのパラメータを編集
    if (!editors_[edit_parameter_index_]->is_empty()) {
        ImGui::Text(ToUTF8("キーフレームの編集").c_str());
        editors_[edit_parameter_index_]->update_select_keyframe();
    }
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
    drag_key_frame_index_ = 999999;
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
