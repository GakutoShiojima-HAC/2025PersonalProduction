// -----------------------------------------------------------------------------------------
//  File        : IState.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/25
//  Description : �X�e�[�g�̃C���^�[�t�F�[�X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef I_STATE_H_
#define I_STATE_H_

class IState {
public:
	IState() = default;

	virtual ~IState() = default;

public:
	/// <summary>
	/// ��ԊJ�n
	/// </summary>
	virtual void enter() = 0;
	
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="delta_time">= �O�t���[������̌o�߃t���[����</param>
	virtual void update(float delta_time) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void draw() const = 0;

	/// <summary>
	/// GUI�̕`��
	/// </summary>
	virtual void draw_gui() const = 0;

	/// <summary>
	/// ��ԏI��
	/// </summary>
	virtual void exit() = 0;

};

#endif