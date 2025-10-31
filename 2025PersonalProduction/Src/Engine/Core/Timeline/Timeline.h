// -----------------------------------------------------------------------------------------
//  File          : Timeline.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : �^�C�����C�����Đ��A�p�����[�^���Ǘ�����N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_H_
#define TIMELINE_H_

#include <string>
#include <vector>
#include "TimelineEditor.h"

class World;
class ITimelineParameter;

class Timeline {
public:
    Timeline() = default;

    ~Timeline();

public:
    void update(float delta_time, float scale_time);

    void clear();

public:
    /// <summary>
    /// ������
    /// </summary>
    /// <param name="world">= ���[���h</param>
    /// <param name="enable_editor">= �G�f�B�^�@�\��L���ɂ��邩�ǂ���</param>
    void init(World* world, bool enable_editor);

    /// <summary>
    /// �^�C�����C���f�[�^��ǂݍ���
    /// </summary>
    /// <param name="json_file"></param>
    void load(const std::string& json_file);

    /// <summary>
    /// �Đ�
    /// </summary>
    /// <param name="name">= �Đ����̖��O</param>
    void play(const std::string& name);

    /// <summary>
    /// ��~
    /// </summary>
    void stop();

    /// <summary>
    /// �Đ������ǂ���
    /// </summary>
    /// <returns></returns>
    bool is_playing() const;

private:
    /// <summary>
    /// �^�C�����C���p�����[�^������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������Ȃ�������nullptr</returns>
    ITimelineParameter* find(const std::string& name);

private:
    std::vector<ITimelineParameter*> timelines_;

    TimelineEditor editor_;

public:
    // �R�s�[�֎~
    Timeline(const Timeline& other) = delete;
    Timeline& operator = (const Timeline& other) = delete;

};

#endif

