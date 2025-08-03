// -----------------------------------------------------------------------------------------
//  File          : CameraTimelineEditor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : �J�����p�^�C�����C���̃G�f�B�^
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_TIMELINE_EDITOR_H_
#define CAMERA_TIMELINE_EDITOR_H_

#include "Engine/Core/Timeline/Editor/TimelineEditor.h"
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"

using CameraKeyFrame = CameraTimeline::CameraKeyFrame;
using CameraTimelineData = CameraTimeline::CameraTimelineData;

class World;

class CameraTimelineEditor : public TimelineEditor {
public:
	CameraTimelineEditor(World* world);

	~CameraTimelineEditor();

public:
	void update(float delta_time) override;

	void clear() override;

private:
	void draw_add_keyframe() override;

	void draw_current_keyframe() override;

	void play() override;

	void save() override;

	void load() override;

	void reset() override;

	void sort_timeline() override;

	void remove_key_frame(GSuint index) override;

	float& get_time(int index) override;

private:
	/// <summary>
	/// �L�[�t���[���̒ǉ�
	/// </summary>
	/// <param name="time">= �^�C�����C����̎���</param>
	/// <param name="target">= ���_�Ƃ���^�[�Q�b�g��</param>
	/// <param name="position">= ���_����̃J�������W</param>
	/// <param name="lookat">= ���_����̒����_</param>
	/// <param name="angle">= �J�����̌X��</param>
	void add_key_frame(float time, const string& target, const GSvector3& position, const GSvector3& lookat, float angle);

	/// <summary>
	/// �J�����̌X���p�x���擾
	/// </summary>
	/// <param name="rotation">= ��]</param>
	/// <returns>= �p�x(�x)</returns>
	float get_tilt_angle(const GSquaternion& rotation) const;

private:	 
	CameraTimelineData* timeline_{ nullptr };

	// ���_�ɂ���^�[�Q�b�g
	string target_{ "" };

};

#endif
