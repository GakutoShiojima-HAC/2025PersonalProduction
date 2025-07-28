// -----------------------------------------------------------------------------------------
//  File          : CameraTimeline.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/25
//  Updated       : 2025/07/28
//  Description   : �J�����p�^�C�����C��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_TIMELINE_H_
#define CAMERA_TIMELINE_H_

#include "Engine/Core/Timeline/ITimeline.h"
#include <gslib.h>

class Camera;

class CameraTimeline : public ITimeline {
public:
	CameraTimeline(World* world, const string& list_json);

	~CameraTimeline() = default;

public:
	void update(float delta_time) override;

	void clear() override;

	void play(const string& name) override;

	void stop() override;

private:
	void end() override;

	void load(const string& name, const string& load_json_path) override;

private:
	// �L�[�t���[��
	struct CameraKeyFrame {
		CameraKeyFrame(
			float time,
			string target,
			GSvector3 position,
			GSvector3 lookat,
			float angle
		) :
			time{ time },
			target{ target },
			position{ position },
			lookat{ lookat },
			angle{ angle }
		{ }

		float time{ 0.0f };						// ����(�b)
		string target{ "" };					// ��_�Ƃ���A�N�^�[ ���݂��Ȃ���΃��[���h���_
		GSvector3 position{ 0.0f, 0.0f, 0.0f };	// ��_����̍��W
		GSvector3 lookat{ 0.0f, 0.0f, 0.0f };	// �����_
		float angle{ 0.0f };					// �p�x
	};

	// �^�C�����C���f�[�^
	class CameraTimelineData {
	public:
		CameraTimelineData(const vector<CameraKeyFrame*>& timeline, float start_transition_time = 0.0f, float end_transition_time = 0.0f);

		~CameraTimelineData();

	public:
		void clear();

	public:
		/// <summary>
		/// �^�C�����C���f�[�^
		/// </summary>
		const vector<CameraKeyFrame*>& get();

	public:
		/// <summary>
		/// �^�C�����C���p�J�����ɑJ�ڂ���܂ł̎���
		/// </summary>
		float start_transition_time() const;

		/// <summary>
		/// ���̃J�����ɑJ�ڂ���܂ł̎���
		/// </summary>
		float end_transition_time() const;

	private:
		// �^�C�����C���f�[�^
		vector<CameraKeyFrame*> timeline_;
		// �^�C�����C���p�J�����ɑJ�ڂ���܂ł̎���
		float start_transition_time_{ 0.0f };
		// ���̃J�����ɑJ�ڂ���܂ł̎���
		float end_transition_time_{ 0.0f };
	};

private:
	/// <summary>
	/// �^�C�����C���f�[�^���Ǘ����ɒǉ�
	/// </summary>
	/// <param name="name">= ���ʖ�</param>
	/// <param name="data">= �f�[�^</param>
	void add(const string& name, CameraTimelineData* data);

	/// <summary>
	/// �^�C�����C���f�[�^������
	/// </summary>
	/// <param name="name">= ���ʖ�</param>
	CameraTimelineData* find(const string& name);

private:
	// �^�C�����C���f�[�^�}�b�v
	unordered_map<string, CameraTimelineData*> timelines_;
	// ���ݍĐ����̃^�C�����C���f�[�^
	CameraTimelineData* current_{ nullptr };
	// �^�C�����C���p�J����
	Camera* camera_{ nullptr };
	// ���̃J����
	Camera* prev_camera_{ nullptr };
};

#endif
