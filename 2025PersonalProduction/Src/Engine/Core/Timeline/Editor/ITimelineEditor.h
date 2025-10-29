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
    /// �L�[�t���[���̒ǉ����̏����l�ݒ�t�B�[���h�̍X�V
    /// </summary>
    virtual void update_keyframe_initial_parameters() { ImGui::Text("None"); }

    /// <summary>
    /// �^�C�����C���r���[�̃L�[�t���[���̍X�V
    /// </summary>
    virtual void update_select_keyframe() = 0;

    /// <summary>
    /// ���O���擾
    /// </summary>
    /// <returns>���O</returns>
    virtual std::string name() const = 0;

    /// <summary>
    /// �L�[�t���[������ł����邩�ǂ���
    /// </summary>
    /// <returns>����ΐ^��ԋp</returns>
    virtual bool is_empty() const = 0;

    /// <summary>
    /// �L�[�t���[���̐�
    /// </summary>
    /// <returns>��</returns>
    virtual unsigned int count_keyframe() const = 0;

    /// <summary>
    /// �ҏW���̃L�[�t���[���C���f�b�N�X���擾
    /// </summary>
    /// <returns>�ҏW���̃L�[�t���[���C���f�b�N�X</returns>
    unsigned int& edit_keyframe() { return edit_keyframe_index_; }

    /// <summary>
    /// �L�[�t���[������L�[�t���[�������݂��鎞�Ԃ��擾
    /// </summary>
    /// <param name="index">= �L�[�t���[���C���f�b�N�X</param>
    /// <returns>����</returns>
    virtual float& get_keyframe_time(unsigned int index) = 0;

    /// <summary>
    /// �^�C�����C�������ԂŃ\�[�g����
    /// </summary>
    virtual void sort_timeline() = 0;

    /// <summary>
    /// �L�[�t���[���̒ǉ�
    /// </summary>
    /// <param name="time">= �ǉ�����</param>
    virtual void add_keyframe(float time) = 0;

    /// <summary>
    /// �L�[�t���[�����폜
    /// </summary>
    /// <param name="index">= �L�[�t���[���C���f�b�N�X</param>
    virtual void remove_keyframe(unsigned int index) = 0;

    /// <summary>
    /// �ۑ��pjson�f�[�^���쐬
    /// </summary>
    /// <returns>ordered_json�̈ꎞ�Q��</returns>
    virtual ordered_json save_data() = 0;

    /// <summary>
    /// �f�[�^��ǂݍ���
    /// </summary>
    /// <param name="j">= �f�[�^�I�u�W�F�N�g</param>
    virtual void load(const json& j) = 0;

    /// <summary>
    /// �Đ�
    /// </summary>
    virtual void play() = 0;

protected:
    // �ҏW���̃L�[�t���[���C���f�b�N�X
    unsigned int edit_keyframe_index_{ 0 };

};

#endif
