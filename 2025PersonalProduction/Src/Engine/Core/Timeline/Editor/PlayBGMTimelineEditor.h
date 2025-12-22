// -----------------------------------------------------------------------------------------
//  File          : PlayBGMTimelineEditor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/26
//  Updated       : 2025/11/26
//  Description   : PlayBGMのエディタ
//  
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYBGM_TIMELINE_EDITOR_H_
#define PLAYBGM_TIMELINE_EDITOR_H_

#include "ITimelineEditor.h"
#include "../Parameters/PlayBGMTimelineParameter.h"

class PlayBGMTimelineEditor : public ITimelineEditor {
public:
    PlayBGMTimelineEditor(PlayBGMTimelineParameter& parameter);

    ~PlayBGMTimelineEditor();

public:
    void clear() override;

    //void update_keyframe_initial_parameters() override;

    void update_select_keyframe() override;

    std::string name() const override;

    float play_time() const override;

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
    PlayBGMTimelineParameter& parameter_;

    // 編集中のデータ
    PlayBGMTimelineParameter::PlayBGMTimelineData* data_{ nullptr };

};

#endif
