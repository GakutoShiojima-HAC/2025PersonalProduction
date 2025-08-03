// -----------------------------------------------------------------------------------------
//  File          : ITimelineEditor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : �^�C�����C���G�f�B�^�̃C���^�[�t�F�[�X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_EDITOR_H_
#define TIMELINE_EDITOR_H_

#include <string>
#include <vector>
#include <algorithm>
#include "GameConfig.h"
#include <imgui/imgui.h>
#include "Lib/imgui_stdlib.h"
#include "Engine/Core/World/World.h"

using namespace std;

typedef	unsigned int GSuint;

// �������Ă��Ȃ�
const int NOMOVE{ -1 };

class TimelineEditor {
public:
	TimelineEditor() = default;

	virtual ~TimelineEditor() = default;

public:
	virtual void update(float delta_time) = 0;

	virtual void clear() = 0;

protected:
	/// <summary>
	/// �ҏW���̃L�[�t���[���l���㉺������
	/// </summary>
	/// <param name="value">= �ړ���</param>
	/// <param name="vec_size">= �L�[�t���[���̗�</param>
	void set_keyframe(int value, int vec_size);

	/// <summary>
	/// �L�[�t���[���̈ړ��{�^����`�悷��
	/// </summary>
	/// <param name="max_size">= �Ǘ����Ă���L�[�t���[���̗�</param>
	void draw_move_keyframe(int max_size);

protected:
	/// <summary>
	/// �^�C�����C���G�f�B�^�̃��C����ʂ�`�悷��
	/// </summary>
	/// <param name="timeline_is_empty">= �^�C�����C���f�[�^�����݂��邩</param>
	/// <param name="file_path">= �^�C�����C���f�[�^�̕ۑ���</param>
	virtual void draw_main_gui(bool timeline_is_empty, const string& file_path);

	/// <summary>
	/// �^�C�����C���f�[�^�̓ǂݍ��݃{�^����`�悷��
	/// </summary>
	/// <param name="file_path">= �^�C�����C���f�[�^�̕ۑ���</param>
	virtual void draw_load(const string& file_path);
	
	/// <summary>
	/// �^�C�����C���f�[�^�̕ۑ��{�^����`�悷��
	/// </summary>
	/// <param name="timeline_is_empty">= �^�C�����C���f�[�^�����݂��邩</param>
	/// <param name="file_path">= �^�C�����C���f�[�^�̕ۑ���</param>
	virtual void draw_save(bool timeline_is_empty, const string& file_path);
	
	/// <summary>
	/// �^�C�����C���f�[�^�̍Đ��{�^����`�悷��
	/// </summary>
	virtual void draw_play();

	/// <summary>
	/// �^�C�����C���f�[�^�̃��Z�b�g�{�^����`�悷��
	/// </summary>
	virtual void draw_reset();

	/// <summary>
	/// �^�C�����C����`�悷��
	/// </summary>
	/// <param name="timeline_is_empty">= �^�C�����C���f�[�^�����݂��邩</param>
	/// <param name="max_size">= �Ǘ����Ă���L�[�t���[���̗�</param>
	virtual void draw_timeline(bool timeline_is_empty, int max_size);

protected:
	/// <summary>
	/// �L�[�t���[���ǉ���ʂ�`�悷��
	/// </summary>
	virtual void draw_add_keyframe() = 0;

	/// <summary>
	/// ���݂̃L�[�t���[���ҏW��ʂ�`�悷��
	/// </summary>
	virtual void draw_current_keyframe() = 0;

	/// <summary>
	/// �^�C�����C�����Đ�����
	/// </summary>
	virtual void play() = 0;

	/// <summary>
	/// �^�C�����C���f�[�^��ۑ�����
	/// </summary>
	virtual void save() = 0;

	/// <summary>
	/// �����̃^�C�����C���f�[�^��ǂݍ���
	/// </summary>
	virtual void load() = 0;

	/// <summary>
	/// �ҏW���̃^�C�����C���f�[�^�����Z�b�g����
	/// </summary>
	virtual void reset() = 0;

	/// <summary>
	/// �^�C�����C���f�[�^���L�[�t���[���̎��ԏ��Ƀ\�[�g����
	/// </summary>
	virtual void sort_timeline() = 0;

	/// <summary>
	/// �w�肵���L�[�t���[�����폜����
	/// </summary>
	/// <param name="index">= �L�[�t���[���ԍ�</param>
	virtual void remove_key_frame(GSuint index) = 0;

	/// <summary>
	/// �L�[�t���[���̎��Ԃ��擾����
	/// </summary>
	/// <param name="index">= �L�[�t���[���ԍ�</param>
	/// <returns>���Ԃ̎Q��</returns>
	virtual float& get_time(int index) = 0;

protected:
	// ���[���h
	World* world_{ nullptr };
	// �ҏW���̃L�[�t���[��
	GSuint current_edit_keyframe_{ 0 };
	// �}�E�X�ňړ����̃L�[�t���[��
	int current_move_keyframe_{ NOMOVE };
	// �L�[�t���[���ǉ�����
	float time_{ 0.0f };
	// �^�C�����C���̕`��J�n����
	float timeline_start_time_{ 0.0f };
	// �ۑ���̃t�@�C����
	string save_filename_{ "" };
	// ���[�h���̃t�@�C����
	string load_filename_{ "" };

protected:
	// �^�C�����C���`��̕�
	const float TIMELINE_MAX_TIME{ 5.0f };
	// �^�C�����C���`��̍���
	const float TIMELINE_HEIGHT{ 40.0f };

public:
	// �R�s�[�֎~
	TimelineEditor(const TimelineEditor& other) = delete;
	TimelineEditor& operator = (const TimelineEditor& other) = delete;

};

#endif
