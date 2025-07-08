// -----------------------------------------------------------------------------------------
//  File        : NullState.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/25
//  Description : �X�e�[�g�}�V�[���G���[���p��X�e�[�g
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef NULL_STATE_H_
#define NULL_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class NullState : public IState {
public:
	virtual void enter() override {}

	virtual void update(float delta_time) override {}

	virtual void draw() const override {}

	virtual void draw_gui() const override {}

	virtual void exit() override {}

};

#endif
