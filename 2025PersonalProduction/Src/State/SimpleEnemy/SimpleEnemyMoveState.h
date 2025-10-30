// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemyMoveState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/24
//  Updated       : 2025/10/24
//  Description   : �V���v���G�l�~�[�̈ړ����
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_MOVE_STATE_H_
#define SIMPLE_ENEMY_MOVE_STATE_H_

#include "State/SimpleEnemy/SimpleEnemyState.h"

class SimpleEnemyMoveState : public SimpleEnemyState {
public:
    SimpleEnemyMoveState(SimpleEnemy& owner);

public:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override;

};

#endif
