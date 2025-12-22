#ifndef I_TIMELINE_EDITOR_H_
#define I_TIMELINE_EDITOR_H_

#include <string>
#include <imgui/imgui.h>
#include "Engine/Utils/MyJson.h"

class ITimelineEditor {
public:
    ITimelineEditor() = default;

    virtual ~ITimelineEditor() = default;

public:
    virtual void clear() = 0;

public:
    /// <summary>
    /// キーフレームの追加時の初期値設定フィールドの更新
    /// </summary>
    virtual void update_keyframe_initial_parameters() { ImGui::Text("None"); }

    /// <summary>
    /// タイムラインビューのキーフレームの更新
    /// </summary>
    virtual void update_select_keyframe() = 0;

    /// <summary>
    /// 名前を取得
    /// </summary>
    /// <returns>名前</returns>
    virtual std::string name() const = 0;

    /// <summary>
    /// 再生タイマーを取得
    /// </summary>
    /// <returns></returns>
    virtual float play_time() const = 0;

    /// <summary>
    /// キーフレームが存在しないかどうか
    /// </summary>
    /// <returns>存在しなければ真を返却</returns>
    virtual bool is_empty() const = 0;

    /// <summary>
    /// キーフレームの数
    /// </summary>
    /// <returns>数</returns>
    virtual unsigned int count_keyframe() const = 0;

    /// <summary>
    /// 編集中のキーフレームインデックスを取得
    /// </summary>
    /// <returns>編集中のキーフレームインデックス</returns>
    unsigned int& edit_keyframe() { return edit_keyframe_index_; }

    /// <summary>
    /// キーフレームからキーフレームが存在する時間を取得
    /// </summary>
    /// <param name="index">= キーフレームインデックス</param>
    /// <returns>時間</returns>
    virtual float& get_keyframe_time(unsigned int index) = 0;

    /// <summary>
    /// タイムラインを時間でソートする
    /// </summary>
    virtual void sort_timeline() = 0;

    /// <summary>
    /// キーフレームの追加
    /// </summary>
    /// <param name="time">= 追加時間</param>
    virtual void add_keyframe(float time) = 0;

    /// <summary>
    /// キーフレームを削除
    /// </summary>
    /// <param name="index">= キーフレームインデックス</param>
    virtual void remove_keyframe(unsigned int index) = 0;

    /// <summary>
    /// 保存用jsonデータを作成
    /// </summary>
    /// <returns>ordered_jsonの一時参照</returns>
    virtual ordered_json save_data() = 0;

    /// <summary>
    /// データを読み込む
    /// </summary>
    /// <param name="j">= データオブジェクト</param>
    virtual void load(const json& j) = 0;

    /// <summary>
    /// 再生
    /// </summary>
    virtual void play() = 0;

protected:
    // 編集中のキーフレームインデックス
    unsigned int edit_keyframe_index_{ 0 };

};

#endif
