#ifndef CAMERA_TIMELINE_EDITOR_H_
#define CAMERA_TIMELINE_EDITOR_H_

#include "ITimelineEditor.h"
#include "../Parameters/CameraTimelineParameter.h"

class CameraTimelineEditor : public ITimelineEditor {
public:
    CameraTimelineEditor(CameraTimelineParameter& parameter);

    ~CameraTimelineEditor();

public:
    void clear() override;

    //void update_keyframe_initial_parameters() override;

    void update_select_keyframe() override;

    std::string name() const override;

    virtual bool is_empty() const override;

    unsigned int count_keyframe() const override;

    float& get_keyframe_time(unsigned int index) override;

    void sort_timeline() override;

    void add_keyframe(float time) override;

    void remove_keyframe(unsigned int index) override;

    ordered_json save_data() override;

    void load(const json& j) override;

    void play() override;

private:
    /// <summary>
    /// カメラの傾き角度を取得
    /// </summary>
    /// <param name="rotation">= 回転</param>
    /// <returns>= 角度(度)</returns>
    float get_tilt_angle(const GSquaternion& rotation) const;

private:
    CameraTimelineParameter& parameter_;

    // 編集中のデータ
    CameraTimelineParameter::CameraTimelineData* data_{ nullptr };

};

#endif
