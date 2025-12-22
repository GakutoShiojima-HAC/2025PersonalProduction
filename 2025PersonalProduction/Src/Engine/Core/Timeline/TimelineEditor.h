#ifndef TIMELINE_EDITOR_H_
#define TIMELINE_EDITOR_H_

#include <vector>
#include <string>

class ITimelineEditor;

class TimelineEditor {
public:
    TimelineEditor() = default;

    ~TimelineEditor();

public:
    void update(float delta_time);

    void clear();

public:
    void add(ITimelineEditor* editor);

private:
    void update_main_gui();

    void update_add_keyframe();

    void update_timeline_view();

private:
    void on_play();

    void on_reset();

    void on_load();

    void on_save();

private:
    /// <summary>
    /// タイムラインエディタを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つからなかったらnullptr</returns>
    ITimelineEditor* find(const std::string& name);

private:
    // ロードするファイル名
    std::string load_file_name_{ "" };
    // 編集中のタイムラインデータの名前
    std::string name_{ "" };
    // タイムラインエディタ
    std::vector<ITimelineEditor*> editors_;
    // 選択中のタイムラインパラメータ
    unsigned int select_parameter_index_{ 0 };
    // 編集中のタイムラインパラメータ
    unsigned int edit_parameter_index_{ 0 };
    // キーフレームを追加する時間
    float add_keyframe_time_{ 0.0f };
    // タイムラインビューの開始時間(左端の時間)
    float timeline_view_start_time_{ 0.0f };
    // ドラッグしていない
    const unsigned int NO_DRAG{ 999999 };
    // ドラッグ中のキーフレームインデックス
    unsigned int drag_key_frame_index_{ NO_DRAG };
    

public:
    // コピー禁止
    TimelineEditor(const TimelineEditor& other) = delete;
    TimelineEditor& operator = (const TimelineEditor& other) = delete;

};

#endif
