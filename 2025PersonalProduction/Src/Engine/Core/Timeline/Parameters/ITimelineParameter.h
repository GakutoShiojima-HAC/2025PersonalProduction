// -----------------------------------------------------------------------------------------
//  File          : ITimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : �^�C�����C���p�����[�^�̃C���^�[�t�F�[�X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef I_TIMELINE_PARAMETER_H_
#define I_TIMELINE_PARAMETER_H_

#include <string>
#include "Engine/Utils/MyJson.h"

typedef	unsigned int GSuint;

class World;

class ITimelineParameter {
public:
    ITimelineParameter(World* world) : world_{ world } {};

    virtual ~ITimelineParameter() = default;

public:
    virtual void update(float delta_time) = 0;

    virtual void clear() = 0;

public:
    /// <summary>
    /// �p�����[�^�̎��ʖ����擾
    /// </summary>
    /// <returns>���ʖ�</returns>
    virtual std::string name() const = 0;

    /// <summary>
    /// �f�[�^��ǂݍ���
    /// </summary>
    /// <param name="name">= �Đ����̖��O</param>
    /// <param name="j">= �f�[�^�I�u�W�F�N�g</param>
    virtual void load(const std::string& name, const json& j) = 0;

    /// <summary>
    /// �w�肵�����O�̃^�C�����C�����Đ�
    /// </summary>
    /// <param name="name">= ���O</param>
    virtual void play(const std::string& name) = 0;

    /// <summary>
    /// �Đ����̃^�C�����C�����~
    /// </summary>
    virtual void stop() = 0;

    /// <summary>
    /// �^�C�����C�����Đ������ǂ���
    /// </summary>
    /// <returns>�Đ����Ȃ�^��ԋp</returns>
    virtual bool is_playing() const { return is_playing_; }

    /// <summary>
    /// ���[���h���擾
    /// </summary>
    /// <returns>�G�f�B�^�p IWorld�ŕԂ�</returns>
    virtual IWorld* get_world() { return world_; }

protected:
    // ���[���h
    World* world_{ nullptr };
    // �Đ������ǂ���
    bool is_playing_{ false };
    // �Đ��^�C�}�[
    float timer_{ 0.0f };
    // ���݂̃L�[�t���[��
    GSuint key_frame_{ 0 };

public:
    // �R�s�[�֎~
    ITimelineParameter(const ITimelineParameter& other) = delete;
    ITimelineParameter& operator = (const ITimelineParameter& other) = delete;

};

#endif
