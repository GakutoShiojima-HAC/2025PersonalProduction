// -----------------------------------------------------------------------------------------
//  File          : CharacterManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : �L�����N�^�[�Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_MANAGER_H_
#define CHARACTER_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Actor/ActorTag.h"

using namespace std;

class Character;

class CharacterManager {
public:
    CharacterManager() = default;

    ~CharacterManager();

public:
    /// <summary>
    /// �L�����N�^�[���Ǘ����ɒǉ�
    /// </summary>
    /// <param name="character">= �L�����N�^�[</param>
    void add(Character* character);

    /// <summary>
    /// �������s�����L�����N�^�[�̍폜
    /// </summary>
    void remove();

    /// <summary>
    /// �Ǘ�������S�ẴL�����N�^�[���폜
    /// </summary>
    void clear();

public:
    /// <summary>
    /// �w�肵�����O�����L�����N�^�[������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������΃L�����N�^�[���A������Ȃ����nullptr</returns>
    Character* find(const string& name) const;

    /// <summary>
    /// �w�肵���^�O�����L�����N�^�[��S�Č���
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>�L�����N�^�[</returns>
    vector<Character*> find_with_tag(const ActorTag tag) const;

    /// <summary>
    /// �Ǘ����Ă���L�����N�^�[�̐�
    /// </summary>
    /// <returns>��</returns>
    int count() const;

    /// <summary>
    /// �w�肵���^�O�����L�����N�^�[�̐�
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>��</returns>
    int count_with_tag(const ActorTag tag) const;

private:
    list<Character*> characters_;

public:
    // �R�s�[�֎~
    CharacterManager(const CharacterManager& other) = delete;
    CharacterManager& operator = (const CharacterManager& other) = delete;

};

#endif
