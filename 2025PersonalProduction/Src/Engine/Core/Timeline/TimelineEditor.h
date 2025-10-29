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
    /// �^�C�����C���G�f�B�^������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������Ȃ�������nullptr</returns>
    ITimelineEditor* find(const std::string& name);

private:
    // ���[�h����t�@�C����
    std::string load_file_name_{ "" };
    // �ҏW���̃^�C�����C���f�[�^�̖��O
    std::string name_{ "" };
    // �^�C�����C���G�f�B�^
    std::vector<ITimelineEditor*> editors_;
    // �I�𒆂̃^�C�����C���p�����[�^
    unsigned int select_parameter_index_{ 0 };
    // �ҏW���̃^�C�����C���p�����[�^
    unsigned int edit_parameter_index_{ 0 };
    // �L�[�t���[����ǉ����鎞��
    float add_keyframe_time_{ 0.0f };
    // �^�C�����C���r���[�̊J�n����(���[�̎���)
    float timeline_view_start_time_{ 0.0f };
    // ���쒆�̃L�[�t���[���C���f�b�N�X
    unsigned int move_key_frame_index_{ 999999 };

public:
    // �R�s�[�֎~
    TimelineEditor(const TimelineEditor& other) = delete;
    TimelineEditor& operator = (const TimelineEditor& other) = delete;

};

#endif
