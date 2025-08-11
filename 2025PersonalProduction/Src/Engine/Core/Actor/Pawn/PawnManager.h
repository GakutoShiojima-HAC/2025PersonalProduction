// -----------------------------------------------------------------------------------------
//  File          : PawnManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : �|�[���Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PAWN_MANAGER_H_
#define PAWN_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Actor/ActorTag.h"

using namespace std;

class Pawn;

class PawnManager {
public:
	PawnManager() = default;

	~PawnManager();

public:
	/// <summary>
	/// �|�[�����Ǘ����ɒǉ�
	/// </summary>
	/// <param name="pawn">= �|�[��</param>
	void add(Pawn* pawn);

	/// <summary>
	/// �������s�����|�[���̍폜
	/// </summary>
	void remove();

	/// <summary>
	/// �Ǘ�������S�Ẵ|�[�����폜
	/// </summary>
	void clear();

public:
    /// <summary>
    /// �w�肵�����O�����|�[��������
    /// </summary>
    /// <param name="name">= ���O</param>
    /// <returns>������΃|�[�����A������Ȃ����nullptr</returns>
    Pawn* find(const string& name) const;

    /// <summary>
    /// �w�肵���^�O�����|�[����S�Č���
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>�|�[��</returns>
    vector<Pawn*> find_with_tag(const ActorTag tag) const;

    /// <summary>
    /// �Ǘ����Ă���|�[���̐�
    /// </summary>
    /// <returns>��</returns>
    int count() const;

    /// <summary>
    /// �w�肵���^�O�����|�[���̐�
    /// </summary>
    /// <param name="tag">= �^�O</param>
    /// <returns>��</returns>
    int count_with_tag(const ActorTag tag) const;

private:
    list<Pawn*> pawns_;

public:
	// �R�s�[�֎~
	PawnManager(const PawnManager& other) = delete;
	PawnManager& operator = (const PawnManager& other) = delete;

};

#endif
