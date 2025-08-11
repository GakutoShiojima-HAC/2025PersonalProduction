// -----------------------------------------------------------------------------------------
//  File          : PlayerState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : �v���C���[�̏�Ԓ�`�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class Player;

class PlayerState : public IState {
public:
    PlayerState(Player& owner) : owner_{ owner } {};

public:
    enum class State {
        /* common */
        Idle,           // �ҋ@ (������󂯕t���Ȃ�)
        Fall,           // ��
        Land,           // ���n

        /* action */
        Move,           // �ړ�
        Interact,       // �C���^���N�g
        Attack,         // �U��
        Skill,          // �X�L��
        Jump,           // �W�����v
        Avoid,          // ���

        /* react */
        Hurt,           // ����
        Dead,           // ���S

        MAX_SIZE	    // �v�f��
    };

protected:
    Player& owner_;

};

using PlayerStateType = PlayerState::State;

#endif
