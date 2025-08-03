// -----------------------------------------------------------------------------------------
//  File          : ITimeline.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/25
//  Updated       : 2025/07/25
//  Description   : �^�C�����C���f�[�^�̃C���^�[�t�F�[�X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef I_TIMELINE_H_
#define I_TIMELINE_H_

#include <unordered_map>	// keyframe�Ǘ��p
#include <string>
#include <vector>
#include "GameConfig.h"

using namespace std;

typedef	unsigned int GSuint;

class World;

class ITimeline {
public:
	ITimeline() = default;

	virtual ~ITimeline() = default;

public:
	virtual void update(float delta_time) = 0;

	virtual void clear() = 0;

	/// <summary>
	/// �^�C�����C���Đ����J�n
	/// </summary>
	/// <param name="name">= �Ǘ���</param>
	virtual void play(const string& name) = 0;

	/// <summary>
	/// �^�C�����C���Đ����~
	/// </summary>
	virtual void stop() = 0;

	/// <summary>
	/// �^�C�����C�����Đ����Ă��邩�ǂ�����ԋp
	/// </summary>
	/// <returns>�Đ����Ȃ�^��ԋp</returns>
	virtual bool is_playing() const { return is_playing_; }

protected:
	/// <summary>
	/// �^�C�����C���Đ����I��
	/// </summary>
	virtual void end() = 0;

protected:
	// ���[���h
	World* world_{ nullptr };
	// ���ݍĐ����Ă���L�[�t���[���ԍ�
	GSint play_frame_{ 0 };
	// �Đ��J�n����̃^�C�}�[
	float timer_{ 0.0f };
	// �Đ�����
	bool is_playing_{ false };

public:
	// �R�s�[�֎~
	ITimeline(const ITimeline& other) = delete;
	ITimeline& operator = (const ITimeline& other) = delete;

};

#endif
