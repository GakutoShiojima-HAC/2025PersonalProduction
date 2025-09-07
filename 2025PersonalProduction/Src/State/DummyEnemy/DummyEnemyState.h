// -----------------------------------------------------------------------------------------
//  File          : DummyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/07
//  Updated       : 2025/09/07
//  Description   : �_�~�[�G�l�~�[�̏�Ԓ�`�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef DUMMY_ENEMY_STATE_H_
#define DUMMY_ENEMY_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class DummyEnemy;

class DummyEnemyState : public IState {
public:
    DummyEnemyState(DummyEnemy& owner) : owner_{ owner } {};

public:
    enum class State {
        /* common */
        Idle,           // �ҋ@ (������󂯕t���Ȃ�)

        /* action */
        Move,           // �ړ�
        Attack,         // �U��

        /* react */
        Dead,           // ���S

        MAX_SIZE	    // �v�f��
    };

protected:
    DummyEnemy& owner_;

};

using DummyEnemyStateType = DummyEnemyState::State;

#endif
