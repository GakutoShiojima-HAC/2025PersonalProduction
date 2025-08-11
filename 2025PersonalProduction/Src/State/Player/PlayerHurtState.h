// -----------------------------------------------------------------------------------------
//  File          : PlayerHurtState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : �v���C���[�̕������
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_HURT_STATE_H_
#define PLAYER_HURT_STATE_H_

#include "State/Player/PlayerState.h"

class PlayerHurtState : public PlayerState {
public:
	PlayerHurtState(Player& owner);

private:
	void enter() override;

	void update(float delta_time) override;

	void draw() const override;

	void draw_gui() const override;

	void exit() override;

};

#endif
