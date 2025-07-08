// -----------------------------------------------------------------------------------------
//  File        : StateMachine.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/25
//  Description : �X�e�[�g�}�V�[���ɂ���ԊǗ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <memory>
#include <unordered_map>
#include "Engine/Core/StateMachine/IState.h"

typedef	unsigned int GSuint;

using namespace std;

class StateMachine {
public:
	StateMachine();

	~StateMachine();

public:
	void update(float delta_time);

	void draw() const;

	void draw_gui() const;

	void clear();

public:
	/// <summary>
	/// ��Ԃ̒ǉ�
	/// </summary>
	/// <param name="state_num">= ��Ԕԍ�</param>
	/// <param name="state">= ���</param>
	void add_state(const GSuint state_num, shared_ptr<IState> state);
	
	/// <summary>
	/// ��Ԃ�ύX
	/// </summary>
	/// <param name="state_num">= �ύX��̏�Ԕԍ�</param>
	void change_state(const GSuint state_num);

	/// <summary>
	/// �w�肵����Ԃ����݂̏�Ԃƈ�v���邩�ǂ���
	/// </summary>
	/// <param name="state_num">= ��Ԕԍ�</param>
	/// <returns>��v����Ȃ�^</returns>
	bool is_current_state(const GSuint state_num) const;

	/// <summary>
	/// ���݂̏�Ԕԍ���ԋp
	/// </summary>
	/// <returns>��Ԕԍ�</returns>
	GSuint get_current_state() const;

	/// <summary>
	/// �w�肵����Ԃ�ԋp
	/// </summary>
	/// <param name="state_num">= ��Ԕԍ�</param>
	/// <returns>������Ώ�Ԃ��A������Ȃ����nullptr</returns>
	IState* get_state(const GSuint state_num) const;

private:
	/// <summary>
	/// �w�肵����Ԃ�����
	/// </summary>
	/// <returns>������Ώ�Ԃ��A������Ȃ����nullptr</returns>
	shared_ptr<IState> find(const GSuint state_num) const;

private:
	// ���
	unordered_map<GSuint, shared_ptr<IState>> state_list_;

	// ���݂̏��
	shared_ptr<IState> current_state_;

	// ���݂̏�Ԕԍ�
	GSuint current_state_num_{ 0 };

public:
	// �R�s�[�֎~
	StateMachine(const StateMachine& other) = delete;
	StateMachine& operator = (const StateMachine& other) = delete;

};

#endif
