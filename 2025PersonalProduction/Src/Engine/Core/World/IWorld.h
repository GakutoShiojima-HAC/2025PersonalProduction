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
class Pawn;
class Character;
class TimelineManager;
class NavMeshSurface;

class IWorld {
public:
    IWorld() = default;

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
    /// �i�r���b�V�����擾
    /// </summary>
    /// <returns>�i�r���b�V��</returns>
    virtual NavMeshSurface* navmesh() = 0;

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

    /// <summary>
    /// �w�肵�����O�����|�[��������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������΃|�[�����A������Ȃ����nullptr</returns>
    virtual Pawn* find_pawn(const string& name) const = 0;

    /// <summary>
    /// �w�肵���^�O�����|�[����S�Č���
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>�|�[��</returns>
    virtual vector<Pawn*> find_pawn_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// �w�肵�����O�����L�����N�^�[������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������΃L�����N�^�[���A������Ȃ����nullptr</returns>
    virtual Character* find_character(const string& name) const = 0;

    /// <summary>
    /// �w�肵���^�O�����L�����N�^�[��S�Č���
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>�L�����N�^�[</returns>
    virtual vector<Character*> find_character_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// �^�C�����C���}�l�[�W���[���擾
    /// </summary>
    /// <returns>�^�C�����C���}�l�[�W���[</returns>
    virtual TimelineManager& timeline() = 0;

    /// <summary>
    /// �U������𐶐�
    /// </summary>
    /// <param name="radius">= ����̔��a</param>
    /// <param name="center">= �������苅�̒��S</param>
    /// <param name="owner">= ������</param>
    /// <param name="damage">= �_���[�W�l</param>
    /// <param name="lifespan">= ����̎���</param>
    /// <param name="delay">= ���肪�L���ɂȂ�܂ł̒x��</param>
    virtual void generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan = 1.0f, float delay = 0.0f) = 0;

    /// <summary>
    /// ������o�p�|�X�g�G�t�F�N�g��`�悷�邩�ǂ���
    /// </summary>
    /// <returns>�Q��</returns>
    virtual bool& enable_avoid_posteffct() = 0;

    /// <summary>
    /// ������o�p�|�X�g�G�t�F�N�g�̐F��ݒ�
    /// </summary>
    /// <param name="color">RGB</param>
    virtual void set_avoid_effect_color(const GSvector3& color) = 0;

public:
	// �R�s�[�֎~
	IWorld(const IWorld& other) = delete;
	IWorld& operator = (const IWorld& other) = delete;

};

#endif
