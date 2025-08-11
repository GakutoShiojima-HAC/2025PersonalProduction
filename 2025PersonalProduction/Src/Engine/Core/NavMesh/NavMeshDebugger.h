// -----------------------------------------------------------------------------------------
//  File          : NavMeshDebugger.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/06
//  Updated       : 2025/08/08
//  Description   : �i�r���b�V���f�o�b�O�N���X
//					���[�g�̕\���A�ړ��̃f�o�b�O���\
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef NAV_MESH_DEBUGGER_H_
#define NAV_MESH_DEBUGGER_H_

#include "Engine/Core/Actor/Actor.h"
#include "Engine/Core/NavMesh/NavMeshAgent.h"

class NavMeshDebugger : public Actor {
public:
	NavMeshDebugger(IWorld* world);

public:
	void update(float delta_time) override;

	void draw() const;

private:
	/// <summary>
	/// �o�H�ɉ����Ĉړ����J�n����
	/// </summary>
	void move_start();

	/// <summary>
	/// �o�H�ɉ����Ĉړ�����
	/// </summary>
	void update_move(float delta_time);

private:
	// �i�r���b�V��
	NavMeshAgent navmesh_{};

	// �J�n�n�_
	GSvector3 start_{ 1.0f, 0.5f, 0.0f };
	// �I���n�_
	GSvector3 end_{ -1.0f, 0.5f, 0.0f };

	// �ړ��ҋ@�����ǂ���
	bool is_move_waiting_{ true };

#ifdef _DEBUG
	// �����o�H������`�悷�邩�ǂ���
	bool draw_line_path_{ false };
#endif
};

#endif
