// -----------------------------------------------------------------------------------------
//  File          : DummyEnemyIdleState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/07
//  Updated       : 2025/09/07
//  Description   : �_�~�[�G�l�~�[�̑ҋ@���
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef DUMMY_ENEMY_IDLE_STATE_H_
#define DUMMY_ENEMY_IDLE_STATE_H_

#include "State/DummyEnemy/DummyEnemyState.h"

class DummyEnemyIdleState : public DummyEnemyState {
public:
	DummyEnemyIdleState(DummyEnemy& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	virtual void draw() const override {}

	virtual void draw_gui() const override {}

	void exit() override;

};

#endif