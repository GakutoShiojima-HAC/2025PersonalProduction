// -----------------------------------------------------------------------------------------
//  File        : CameraManager.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/10
//  Updated     : 2025/07/10
//  Description : �Q�[�����ɓo�ꂷ��J�������Ǘ�����N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_

#include <unordered_map>
#include "Camera/CameraTag.h"

using namespace std;

class Camera;

class CameraManager {
public:
	CameraManager() = default;

	virtual ~CameraManager();

public:
	void update(float delta_time);

	virtual void draw() const;

	/// <summary>
	/// �J�������Ǘ����ɒǉ�
	/// </summary>
	/// <param name="camera">�J����</param>
	void add(Camera* camera);
	
	/// <summary>
	/// �Ǘ����̑S�ẴJ�������Ǘ��Ώۂ���O��
	/// </summary>
	void clear();

	/// <summary>
	/// ���ݎg�p���Ă���J������ԋp
	/// </summary>
	/// <returns>�J����</returns>
	Camera* current() const;

	/// <summary>
	/// �J�ڌ��Ɏg�p���Ă���J������ԋp
	/// </summary>
	/// <returns>�g�p���Ă���΃J�������A���Ă��Ȃ����nullptr</returns>
	Camera* prev() const;

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="tag">�J�����^�O</param>
	/// <returns>������΃J�������A������Ȃ����nullptr</returns>
	Camera* find(const CameraTag tag) const;

	/// <summary>
	/// ���݂̃J��������w�肵���J�����ɑJ��
	/// </summary>
	/// <param name="to">= �J�ڐ�</param>
	/// <param name="time">= �J�ڂɂ����鎞��</param>
	void transition(Camera* to, float time = 0);

	/// <summary>
	/// �w�肵���J��������w�肵���J�����ɑJ��
	/// </summary>
	/// <param name="from">= �J�ڌ�</param>
	/// <param name="to">= �J�ڐ�</param>
	/// <param name="time">= �J�ڂɂ����鎞��</param>
	void transition(Camera* from, Camera* to, float time = 0);

protected:
	/// <summary>
	/// �Ǘ����̎������s�����J�������Ǘ��Ώۂ���O��
	/// </summary>
	void remove();

	/// <summary>
	/// �J�������Ȃ��Ƃ��̕`�揈��
	/// </summary>
	virtual void draw_empty() const;

	/// <summary>
	/// �J�����̍��W�A�����A�p�x���擾
	/// </summary>
	/// <param name="pos">= �J�������W</param>
	/// <param name="at">= �J��������</param>
	/// <param name="up">= �J�����p�x</param>
	void camera_lookat(GSvector3& pos, GSvector3& at, GSvector3& up) const;

protected:
	// �J�����}�b�v
	unordered_map<GSuint, Camera*> cameras_;
	// ���ݎg�p���Ă���J����
	Camera* current_{ nullptr };
	// �g�����W�V�����p�̈ȑO�Ɏg���Ă����J����
	Camera* prev_{ nullptr };

	// �g�����W�V�����ɂ����鎞��
	float transition_time_{ 0.0f };
	// �g�����W�V�����^�C�}�[
	float transition_timer_{ 0.0f };

public:
	// �R�s�[�֎~
	CameraManager(const CameraManager& other) = delete;
	CameraManager& operator = (const CameraManager& other) = delete;

};

#endif
