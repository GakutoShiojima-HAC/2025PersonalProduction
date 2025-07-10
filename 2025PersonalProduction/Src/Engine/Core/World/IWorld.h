// -----------------------------------------------------------------------------------------
//  File        : IWorld.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/07/10
//  Description : ���[���h�Ǘ��N���X�̃C���^�[�t�F�[�X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef I_WORLD_H_
#define I_WORLD_H_

#include <string>
#include <vector>
#include "Camera/CameraTag.h"
#include "Actor/ActorTag.h"

using namespace std;

class Field;
class FieldActor;
class Light;
class Camera;
class Actor;

class IWorld {
public:
	virtual ~IWorld() = default;

public:
	/// <summary>
	/// �t�B�[���h�̎擾
	/// </summary>
	/// <returns>�t�B�[���h</returns>
	virtual Field* get_field() = 0;

	/// <summary>
	/// ���C�g�̎擾
	/// </summary>
	/// <returns>���C�g</returns>
	virtual Light* get_light() = 0;

    /// <summary>
    /// �J������ǉ�
    /// </summary>
    /// <param name="camera">�J����</param>
    virtual void add_camera(Camera* camera) = 0;

    /// <summary>
    /// ���ݎg�p���Ă���J�������擾
    /// </summary>
    /// <returns>�J����</returns>
    virtual Camera* get_camera() const = 0;

    /// <summary>
    /// �J����������
    /// </summary>
    /// <param name="tag">= �J�����^�O</param>
    /// <returns>������΃J�������A������Ȃ����nullptr</returns>
    virtual Camera* find_camera(const CameraTag tag) const = 0;

    /// <summary>
    /// ���݂̃J��������w�肵���J�����ɑJ��
    /// </summary>
    /// <param name="to">= �J�ڐ�</param>
    /// <param name="time">= �J�ڂɂ����鎞��</param>
    virtual void camera_transition(Camera* to, float time = 0) = 0;

    /// <summary>
    /// �w�肵���J��������w�肵���J�����ɑJ��
    /// </summary>
    /// <param name="from">= �J�ڌ�</param>
    /// <param name="to">= �J�ڐ�</param>
    /// <param name="time">= �J�ڂɂ����鎞��</param>
    virtual void camera_transition(Camera* from, Camera* to, float time = 0) = 0;

    /// <summary>
    /// �w�肵�����O�����A�N�^�[������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������΃A�N�^�[���A������Ȃ����nullptr</returns>
    virtual Actor* find_actor(const string& name) const = 0;

    /// <summary>
    /// �w�肵���^�O�����A�N�^�[��S�Č���
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>�A�N�^�[</returns>
    virtual vector<Actor*> find_actor_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// �Ǘ����Ă���A�N�^�[�̐�
    /// </summary>
    /// <returns>��</returns>
    virtual int count_actor() const = 0;

    /// <summary>
    /// �w�肵���^�O�����A�N�^�[�̐�
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>��</returns>
    virtual int count_actor_with_tag(const ActorTag tag) const = 0;

public:
	// �R�s�[�֎~
	IWorld(const IWorld& other) = delete;
	IWorld& operator = (const IWorld& other) = delete;

};

#endif
