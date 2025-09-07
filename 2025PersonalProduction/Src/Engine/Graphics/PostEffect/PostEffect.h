// -----------------------------------------------------------------------------------------
//  File          : PostEffect.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/07
//  Updated       : 2025/09/07
//  Description   : �|�X�g�G�t�F�N�g
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef POST_EFFECT_H_
#define POST_EFFECT_H_

#include "Engine/Core/Setting/Setting.h"

// �����_�[�^�[�Q�b�g
enum {
	Rt_Base,				// �x�[�X�̃����_�[�^�[�Q�b�g

	Rt_AvoidMask,			// ������o�p�̃}�X�N�p
	Rt_AvoidEffect,			// ������o�̃|�X�g�G�t�F�N�g�p

	Rt_BloomExtract,		// ���P�x�e�N�Z�����o

	Rt_GaussianBlurH1,		// ���������u���[(1/4�k���o�b�t�@)
	Rt_GaussianBlurV1,		// ���������u���[(1/4�k���o�b�t�@)
	Rt_GaussianBlurH2,		// ���������u���[(1/8�k���o�b�t�@)
	Rt_GaussianBlurV2,		// ���������u���[(1/8�k���o�b�t�@)
	Rt_GaussianBlurH3,		// ���������u���[(1/16�k���o�b�t�@)
	Rt_GaussianBlurV3,		// ���������u���[(1/16�k���o�b�t�@)
	Rt_GaussianBlurH4,		// ���������u���[(1/32�k���o�b�t�@)
	Rt_GaussianBlurV4,		// ���������u���[(1/32�k���o�b�t�@)

	Rt_BloomCombine,		// �u���[���G�t�F�N�g����

	Rt_FXAA,				// �A���`�G�C���A�V���O����
};

// �V�F�[�_�[
enum {
	Shader_AvoidEffect,		// ������o
	Shader_BloomExtract,    // ���P�x�s�N�Z�����o
	Shader_GaussianBlur,    // �K�E�V�A���u���[
	Shader_BloomCombine,    // �u���[���G�t�F�N�g����
	Shader_FXAA,			// �A���`�G�C���A�V���O
};

class PostEffect {
public:
	PostEffect() = default;

	~PostEffect() = default;

public:
	void init();

	void clear();
	
public:
	/// <summary>
	/// �|�X�g�G�t�F�N�g�p�����_�[�^�[�Q�b�g�ɕ`����J�n����
	/// </summary>
	void start() const;

	/// <summary>
	/// �|�X�g�G�t�F�N�g�p�����_�[�^�[�Q�b�g�ւ̕`����I������
	/// </summary>
	void end() const;

	/// <summary>
	/// ���ʂ�`�悷��
	/// </summary>
	void draw() const;

	/// <summary>
	/// ������o�p�����_�[�^�[�Q�b�g�ɕ`����J�n����
	/// </summary>
	void start_avoid_effect() const;

	/// <summary>
	/// ������o�p�����_�[�^�[�Q�b�g�ւ̕`����I������
	/// </summary>
	void end_avoid_effect() const;

public:
	/// <summary>
	/// ������o�G�t�F�N�g��L���ɂ���
	/// </summary>
	/// <returns>�Q��</returns>
	bool& enable_avoid_effect();
	
	/// <summary>
	/// ������o��`�撆���ǂ���
	/// </summary>
	/// <returns>�`�撆�Ȃ�^��ԋp</returns>
	bool is_draw_avoid_effect() const;

	/// <summary>
	/// ������o�̐F��ݒ�
	/// </summary>
	/// <param name="color">RGB</param>
	void set_avoid_color(const GSvector3& color);

private:
	/// <summary>
	/// ����G�t�F�N�g
	/// </summary>
	/// <param name="source">= ���摜�̃����_�[�^�[�Q�b�g</param>
	/// <returns>�|�X�g�G�t�F�N�g��K�p���������_�[�^�[�Q�b�g</returns>
	GSuint apply_avoid_effect(GSuint source, GSuint mask) const;

	/// <summary>
	/// �K�E�V�A���u���[
	/// </summary>
	/// <param name="source">= ���摜�̃����_�[�^�[�Q�b�g</param>
	/// <param name="size">= �k���o�b�t�@�̃T�C�Y</param>
	/// <param name="blur_h">= ���������u���[�p�̃����_�[�^�[�Q�b�g</param>
	/// <param name="blur_v">= ���������u���[�p�̃����_�[�^�[�Q�b�g</param>
	void gaussian_blur(GSuint source, GSvector2 size, GSuint blur_h, GSuint blur_v) const;

	/// <summary>
	/// �u���[���Ɏg�����P�x�e�N�Z���̒��o
	/// </summary>
	GSuint bloom_extract(GSuint source) const;

	/// <summary>
	/// �u���[���G�t�F�N�g������
	/// </summary>
	GSuint bloom_combine(GSuint source) const;

	/// <summary>
	/// �A���`�G�C���A�V���O
	/// </summary>
	GSuint apply_fxaa(GSuint source) const;

private:
	GLint width_{ 0 };
	GLint height_{ 0 };

	Setting& setting_ = Setting::get_instance();

	// ������o�����ǂ���
	bool is_avoid_effect_{ false };
	// ������o�Ɏg���F
	GSvector3 avoid_color_{ 0.2f, 0.4f, 1.0f };

public:
	// �R�s�[�֎~
	PostEffect(const PostEffect& other) = delete;
	PostEffect& operator = (const PostEffect& other) = delete;

};

#endif
