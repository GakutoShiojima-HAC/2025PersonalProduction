// -----------------------------------------------------------------------------------------
//  File          : ActorManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �A�N�^�[�Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Actor/ActorTag.h"

using namespace std;

class Actor;

class ActorManager {
public:
	ActorManager() = default;

	virtual ~ActorManager();

public:
    /// <summary>
    /// �A�N�^�[���Ǘ����ɒǉ�
    /// </summary>
    /// <param name="actor">= �A�N�^�[</param>
    void add(Actor* actor);

    /// <summary>
    /// �A�N�^�[�̍X�V
    /// </summary>
    void update(float delta_time);

    /// <summary>
    /// �A�N�^�[�̍X�V update��collide�̌�ɌĂ΂��
    /// </summary>
    void late_update(float delta_time);

    /// <summary>
    /// �A�N�^�[�̕`��
    /// </summary>
    void draw() const;

    /// <summary>
    /// �A�N�^�[�̔������`��
    /// </summary>
    void draw_tranparent() const;

    /// <summary>
    /// �A�N�^�[��GUI�`��
    /// </summary>
    void draw_gui() const;

    /// <summary>
    /// �A�N�^�[�̏Փ˔���
    /// </summary>
    void collide();

    /// <summary>
    /// �������s�����A�N�^�[�̍폜
    /// </summary>
    void remove();

    /// <summary>
    /// �w�肵�����O�����A�N�^�[������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������΃A�N�^�[���A������Ȃ����nullptr</returns>
    Actor* find(const string& name) const;

    /// <summary>
    /// �w�肵���^�O�����A�N�^�[��S�Č���
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>�A�N�^�[</returns>
    vector<Actor*> find_with_tag(const ActorTag tag) const;

    /// <summary>
    /// �Ǘ����Ă���A�N�^�[�̐�
    /// </summary>
    /// <returns>��</returns>
    int count() const;

    /// <summary>
    /// �w�肵���^�O�����A�N�^�[�̐�
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>��</returns>
    int count_with_tag(const ActorTag tag) const;

    /// <summary>
    /// �Ǘ����Ă���A�N�^�[������
    /// </summary>
    void clear();

protected:
    // �A�N�^�[���X�g
    list<Actor*> actors_;

public:
    // �R�s�[�֎~
    ActorManager(const ActorManager& other) = delete;
    ActorManager& operator = (const ActorManager& other) = delete;

};

#endif
