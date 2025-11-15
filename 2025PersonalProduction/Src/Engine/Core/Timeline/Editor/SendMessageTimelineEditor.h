#ifndef SEND_MESSAGE_TIMELINE_EDITOR_H_
#define SEND_MESSAGE_TIMELINE_EDITOR_H_

#include "ITimelineEditor.h"
#include "../Parameters/SendMessageTimelineParameter.h"

class SendMessageTimelineEditor : public ITimelineEditor {
public:
    SendMessageTimelineEditor(SendMessageTimelineParameter& parameter);

    ~SendMessageTimelineEditor();

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
    SendMessageTimelineParameter& parameter_;

    // 編集中のデータ
    SendMessageTimelineParameter::SendMessageTimelineData* data_{ nullptr };

};

#endif
