// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemyState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/24
//  Updated       : 2025/10/24
//  Description   : �V���v���G�l�~�[�̏�Ԓ�`�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_STATE_H_
#define SIMPLE_ENEMY_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class SimpleEnemy;

class SimpleEnemyState : public IState {
public:
    SimpleEnemyState(SimpleEnemy& owner) : owner_{ owner } {};

public:
    enum class State {
        /* common */
        Idle,           // �ҋ@ (������󂯕t���Ȃ�)

        /* action */
        Search,         // �^�[�Q�b�g��T��
        Find,           // �^�[�Q�b�g�֌���
        Move,           // �ړ�
        Attack,         // �U��

        /* react */
        Hurt,           // ����
        Dead,           // ���S

        MAX_SIZE	    // �v�f��
    };

protected:
    SimpleEnemy& owner_;

};

using SimpleEnemyStateType = SimpleEnemyState::State;

#endif
