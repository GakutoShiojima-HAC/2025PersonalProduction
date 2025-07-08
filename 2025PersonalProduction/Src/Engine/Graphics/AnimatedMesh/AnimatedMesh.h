// -----------------------------------------------------------------------------------------
//  File          : AnimatedMesh.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �X�L�j���O���b�V�����A�j���[�V����������N���X
//					�A�j���[�V�����C�x���g�Ή�
//					�A�j���[�V�������C���[�Ή�
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <vector>
#include <functional>
#include <vector>
#include <memory>

using namespace std;

class GStransform;

struct AnimationEvent {
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="motion">= �C�x���g�𔭉΂���Ώۂ̃��[�V�����ԍ�</param>
	/// <param name="time">= �C�x���g�𔭉΂��鎞��</param>
	/// <param name="callback">= �C�x���g����</param>
	AnimationEvent(GSuint motion, GSfloat time, std::function<void()> callback) :
		motion_{ motion },
		time_{ time },
		callback_{ callback } {
	}
	// �C�x���g�𔭐������郂�[�V�����ԍ�
	GSuint motion_;
	// �C�x���g�𔭐�������^�C�~���O
	GSfloat time_;
	// �C�x���g�������̃R�[���o�b�N
	std::function<void()> callback_;

};

class AnimatedMesh {
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="mesh">= ���b�V���ԍ�</param>
	/// <param name="skeleton">= �X�P���g���ԍ�</param>
	/// <param name="animation">= �A�j���[�V�����ԍ�</param>
	/// <param name="motion">= ������Ԃ̃��[�V�����ԍ�</param>
	/// <param name="loop">= ������Ԃ̃��[�V���������[�v���邩�ǂ���</param>
	/// <param name="layer_count">= �A�j���[�V�������C���[��</param>
	/// <param name="bone_count">= �g�p���Ă���{�[���̐�</param>
	AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true, GSuint layer_count = 1, GSuint bone_count = 256);
	
	~AnimatedMesh() = default;

public:
	void update(float delta_time);

	void draw() const;

public:
	/// <summary>
	/// ���[�V�����̕ύX�i���C���[�w��Ȃ��j
	/// </summary>
	/// <param name="motion">= ���[�V�����ԍ�</param>
	/// <param name="loop">= ���[�v���邩�ǂ���</param>
	void change_motion(GSuint motion, bool loop = true);

	/// <summary>
	/// ���[�V�����̕ύX�i���C���[�w�肠��j
	/// </summary>
	/// <param name="layer">= ���C���[�ԍ�</param>
	/// <param name="motion">= ���[�V�����ԍ�</param>
	/// <param name="loop">= ���[�v���邩�ǂ���</param>
	void change_motion(GSuint layer, GSuint motion, bool loop = true);
	
	/// <summary>
	/// ���C���[�C���f�b�N�X�̐ݒ�
	/// </summary>
	/// <param name="layer_index">= ���C���[�ԍ�</param>
	/// <param name="layer_bones">= ���C���[�������{�[���ԍ�</param>
	/// <param name="layer_bones_count">= ���C���[�������{�[���ԍ��̐�</param>
	void set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count);

	/// <summary>
	/// �ϊ��s���ݒ肷��
	/// </summary>
	/// <param name="matrix">= ���[���h�ϊ��s��</param>
	void transform(const GSmatrix4& matrix);

	/// <summary>
	/// ���ݍĐ����Ă��郂�[�V�����̏I�����Ԃ��擾
	/// </summary>
	/// <param name="layer">= ���C���[�ԍ�</param>
	/// <returns>�I������</returns>
	float motion_end_time(GSuint layer = 0) const;

	/// <summary>
	/// ���[�V�����Đ����I�����Ă��邩
	/// </summary>
	/// <param name="layer">= ���C���[�ԍ�</param>
	/// <returns>�I�����Ă�����^�A���[�v�Ȃ�U</returns>
	bool is_motion_end(GSuint layer = 0) const;

	/// <summary>
	/// �{�[���̃��[���h�ϊ��s����擾
	/// </summary>
	/// <param name="bone_no">= �{�[���ԍ�</param>
	/// <returns>���[���h�ϊ��s��</returns>
	GSmatrix4 bone_matrices(int bone_no) const;
	
	/// <summary>
	/// �{�[���̃��[�J���ϊ��s����擾
	/// </summary>
	/// <param name="bone_no">= �{�[���ԍ�</param>
	/// <returns>���[�J���ϊ��s��</returns>
	GSmatrix4 bone_local_matrices(int bone_no) const;
	
	/// <summary>
	/// ���݂̃��[�V�����̑��Đ����Ԃ��擾
	/// </summary>
	/// <param name="layer">= ���C���[�ԍ�</param>
	/// <returns>���Đ�����</returns>
	float current_motion_time(GSuint layer = 0) const;
	
	/// <summary>
	/// ���݂̃��[�V�����̍Đ����Ԃ�ݒ�
	/// </summary>
	/// <param name="time">= �Đ�����������</param>
	/// <param name="layer">= ���C���[�ԍ�</param>
	void current_motion_time(float time, GSuint layer = 0);

	/// <summary>
	/// ���[�g���[�V�����̓K�p
	/// </summary>
	/// <param name="transform">= �g�����X�t�H�[��</param>
	void apply_root_motion(GStransform& transform);

	/// <summary>
	/// �A�j���[�V�����C�x���g��ǉ��i���C���[�w��Ȃ��j
	/// </summary>
	/// <param name="motion">= �C�x���g�𔭉΂���Ώۂ̃��[�V�����ԍ�</param>
	/// <param name="time">= �C�x���g�𔭉΂��鎞��</param>
	/// <param name="callback">= �C�x���g����</param>
	void add_animation_event(GSuint motion, GSfloat time, function<void()> callback);

	/// <summary>
	/// �A�j���[�V�����C�x���g��ǉ��i���C���[�w�肠��j
	/// </summary>
	/// <param name="layer">= ���C���[�ԍ�</param>
	/// <param name="motion">= �C�x���g�𔭉΂���Ώۂ̃��[�V�����ԍ�</param>
	/// <param name="time">= �C�x���g�𔭉΂��鎞��</param>
	/// <param name="callback">= �C�x���g����</param>
	void add_animation_event(GSuint layer, GSuint motion, GSfloat time, function<void()> callback);

private:
	class Animation {
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="animation">= �A�j���[�V�����ԍ�</param>
		/// <param name="motion">= ������Ԃ̃��[�V�����ԍ�</param>
		/// <param name="loop">= ������Ԃ̃��[�V���������[�v���邩�ǂ���</param>
		Animation(GSuint animation = 0, GSuint motion = 0, bool loop = true);

	public:
		void update(float delta_time);

	public:
		/// <summary>
		/// ���[�V�����̕ύX
		/// </summary>
		/// <param name="motion">= ���[�V�����ԍ�</param>
		/// <param name="loop">= ���[�v���邩�ǂ���</param>
		void change_motion(GSuint motion, bool loop = true);

		/// <summary>
		/// �Đ����̃��[�V�����ԍ�
		/// </summary>
		/// <returns>���[�V�����ԍ�</returns>
		GSuint motion_num() const;

		/// <summary>
		/// ���ݍĐ����Ă��郂�[�V�����̏I�����Ԃ��擾
		/// </summary>
		/// <returns>�I������</returns>
		float motion_end_time() const;

		/// <summary>
		/// ���݂̃��[�V�����̑��Đ����Ԃ��擾
		/// </summary>
		/// <returns>���Đ�����</returns>
		float current_motion_time() const;

		/// <summary>
		/// ���݂̃��[�V�����̍Đ����Ԃ�ݒ�
		/// </summary>
		/// <param name="time">= �Đ�����������</param>
		void current_motion_time(float time);

		/// <summary>
		/// ���[�V�����Đ����I�����Ă��邩
		/// </summary>
		/// <returns>�I�����Ă�����^�A���[�v�Ȃ�U</returns>
		bool is_motion_end() const;

		/// <summary>
		/// �{�[���̃��[�J���ϊ��s����擾
		/// </summary>
		/// <param name="bone_no">= �{�[���ԍ�</param>
		/// <returns>���[�J���ϊ��s��</returns>
		GSmatrix4 local_bone_matrix(GSuint bone_no) const;

		/// <summary>
		/// �{�[�������擾
		/// </summary>
		/// <returns>�{�[����</returns>
		GSuint bone_count() const;

		/// <summary>
		/// ���[�g���[�V�����̓K�p
		/// </summary>
		/// <param name="transform">= �g�����X�t�H�[��</param>
		void root_motion(GStransform& transform);

		/// <summary>
		/// �A�j���[�V�����C�x���g��ǉ�
		/// </summary>
		/// <param name="motion">= �C�x���g�𔭉΂���Ώۂ̃��[�V�����ԍ�</param>
		/// <param name="time">= �C�x���g�𔭉΂��鎞��</param>
		/// <param name="callback">= �C�x���g����</param>
		void add_event(GSuint motion, GSfloat time, function<void()> callback);

	private:
		// �A�j���[�V����
		GSuint animation_{ 0 };
		// ���[�V�����ԍ�
		GSuint motion_{ 0 };
		// ���[�V�����^�C�}�[
		GSfloat motion_timer_{ 0.0f };
		// �O��t���[���̃��[�V�����^�C�}�[
		GSfloat prev_timer_;
		// ���[�V�������[�v�t���O
		bool motion_loop_{ true };
		// �O��Đ��������[�V�����ԍ�
		GSuint prev_motion_{ 0 };
		// �O��Đ������ŏI�A�j���[�V�����^�C�}�[
		GSfloat prev_motion_timer_{ 0.0f };
		// ��ԃA�j���[�V�����^�C�}�[
		GSfloat lerp_timer_{ 0.0f };
		// �����̃A�j���[�V�����C�x���g���i�[���邽�߂�vector
		vector<shared_ptr<AnimationEvent>> events_;
	};

private:
	/// <summary>
	/// �����̃��C���[���������[�V�������Đ����Ă���ꍇ�Đ����Ԃ𓯊�������
	/// </summary>
	/// <param name="check_layer">= ���C���[�ԍ�</param>
	void sync_layers_playing_same_motion(GSuint check_layer);

	/// <summary>
	/// �����̃��C���[���������[�V�������Đ����Ă��邩
	/// </summary>
	/// <param name="layer">= ���C���[�ԍ�</param>
	/// <param name="layers">= �������[�V�������Đ����Ă��郌�C���[�ԍ�</param>
	/// <returns>�Đ����Ă�����^</returns>
	bool is_multiple_layers_playing_same_motion(GSuint check_layer, vector<GSuint>& layers) const;

private:
	// ���b�V��
	GSuint mesh_;
	// �X�P���g��
	GSuint skeleton_;
	// �A�j���[�V�������C���[
	vector<Animation> animation_layers_;
	// �A�j���[�V�������C���[�C���f�b�N�X
	vector<GSuint> animation_layer_indices_;
	// �{�[���̃��[�J���ϊ��s��
	vector<GSmatrix4> local_bone_matrices_;
	// �{�[���̕ϊ��s��
	vector<GSmatrix4> bone_matrices_;
	// ���W�ϊ��s��
	GSmatrix4 transform_;

};

#endif
