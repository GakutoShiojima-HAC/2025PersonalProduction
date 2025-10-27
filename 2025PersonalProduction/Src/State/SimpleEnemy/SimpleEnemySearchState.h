// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemySearchState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/24
//  Updated       : 2025/10/24
//  Description   : �V���v���G�l�~�[�̓G�����ҋ@���
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_SEARCH_STATE_H_
#define SIMPLE_ENEMY_SEARCH_STATE_H_

#include "State/SimpleEnemy/SimpleEnemyState.h"

class SimpleEnemySearchState : public SimpleEnemyState {
public:
    SimpleEnemySearchState(SimpleEnemy& owner);

public:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

private:
    bool to_find_state_loop_{ false };

    GSuint to_find_state_motion_{ 0 };

    float move_timer_{ 0.0f };

    GSvector3 rotate_foward_{ 0.0f, 0.0f, 0.0f };

};

#endif
