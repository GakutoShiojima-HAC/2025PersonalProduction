// -----------------------------------------------------------------------------------------
//  File          : NavMeshAgent.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/05
//  Updated       : 2025/10/30
//  Description   : �A�N�^�[���i�r���b�V���ɉ����Ĉړ�������@�\�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef NAV_MESH_AGENT_H_
#define NAV_MESH_AGENT_H_

#include <vector>
#include <gslib.h>
#include "Engine/Core/Actor/Actor.h"

using namespace std;

class NavMeshSurface;

class NavMeshAgent {
public:
	NavMeshAgent(Actor* target = nullptr, NavMeshSurface* navmesh = nullptr);

	~NavMeshAgent();

public:
	/// <summary>
	/// �ړ��Ώۂ��o�H�ɉ����Ĉړ�������
	/// </summary>
	/// <param name="move_speed">= �ړ����x</param>
	/// <param name="rotate_angle">= �����]�����̉�]��</param>
	void update_move(float delta_time, float move_speed, float rotate_angle);
	
	/// <summary>
	/// �o�H��`��
	/// </summary>
	void draw_path() const;

	/// <summary>
	/// �o�H�T��
	/// </summary>
	/// <param name="start">= �J�n�n�_</param>
	/// <param name="end">= �I���n�_</param>
    /// <param name="offset_ratio">= �ǂ���ǂꂾ�������� 0.0~1.0�Ŏw��</param>
	/// <returns>�T���ɐ���������^��ԋp</returns>
	bool find_path(const GSvector3& start, const GSvector3& end, float offset_ratio = 1.0f);

	/// <summary>
	/// �o�H���������Ă��邩
	/// </summary>
	/// <returns>�������Ă�����^��ԋp</returns>
	bool found_path() const;

	/// <summary>
	/// �ړ����I�����Ă��邩�ǂ���
	/// </summary>
	/// <returns>�I�����Ă�����^��ԋp</returns>
	bool end_move() const;

	/// <summary>
	/// �o�H
	/// </summary>
	/// <returns>�o�H</returns>
	const vector<GSvector3>& path();

	/// <summary>
	/// �ړ������Z�b�g����
	/// </summary>
	void reset_move();

private:
	// �i�r���b�V��
	NavMeshSurface* navmesh_{ nullptr };
	// �o�H
	vector<GSvector3> path_;
	// �ǂ��܂Ői�񂾂�
	GSuint path_index_{ 0 };
    // �ĒT���^�C�}�[
    float re_find_timer_{ 0.0f };
    // �S�[���n�_
    GSvector3 goal_position_{ 0.0f, 0.0f, 0.0f };

	// �ړ��Ώۂ̃A�N�^�[
	Actor* target_{ nullptr };

};

#endif
