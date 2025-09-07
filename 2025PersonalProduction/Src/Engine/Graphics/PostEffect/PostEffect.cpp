#include "Engine/Graphics/PostEffect/PostEffect.h"
#include <gslib.h>

// �u���[���G�t�F�N�g�̑Ώۂɂ���e�N�Z���̋P�x�̂������l
const float BLOOM_THRESHOLD{ 0.9f };
// �u���[���G�t�F�N�g�̋���
const float BLOOM_INTENSITY{ 0.2f };

void PostEffect::init() {
    // ���݂̃r���[�|�[�g�̃T�C�Y���擾 [0]x���W, [1]:y���W, [2]:��, [3]:����
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    width_ = viewport[2];
    height_ = viewport[3];

    // �x�[�X�̃����_�[�^�[�Q�b�g�̍쐬
    gsCreateRenderTarget(Rt_Base, width_, height_, GS_TRUE, GS_TRUE, GS_TRUE);
    // ������o�p�̃����_�[�^�[�Q�b�g�̍쐬
    gsCreateRenderTarget(Rt_AvoidMask, width_, height_, GS_TRUE, GS_TRUE, GS_TRUE);
    gsCreateRenderTarget(Rt_AvoidEffect, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);
    // ���P�x���o�V�F�[�_�[�p�̃����_�[�^�[�Q�b�g�̍쐬(1/4�T�C�Y)
    gsCreateRenderTarget(Rt_BloomExtract, width_ / 4, height_ / 4, GS_TRUE, GS_FALSE, GS_TRUE);
    // �K�E�V�A���u���[�p�̏k���o�b�t�@���쐬
    gsCreateRenderTarget(Rt_GaussianBlurH1, width_ / 4, height_ / 4, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV1, width_ / 4, height_ / 4, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurH2, width_ / 8, height_ / 8, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV2, width_ / 8, height_ / 8, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurH3, width_ / 16, height_ / 16, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV3, width_ / 16, height_ / 16, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurH4, width_ / 32, height_ / 32, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV4, width_ / 32, height_ / 32, GS_TRUE, GS_FALSE, GS_TRUE);
    // �u���[�������V�F�[�_�[�p�̃����_�[�^�[�Q�b�g�̍쐬
    gsCreateRenderTarget(Rt_BloomCombine, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);
    // �A���`�G�C���A�V���O�p�̃����_�[�^�[�Q�b�g�̍쐬
    gsCreateRenderTarget(Rt_FXAA, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);

    // �V�F�[�_�[�̓ǂݍ���
    gsLoadShader(Shader_AvoidEffect, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/AvoidEffect.frag");
    gsLoadShader(Shader_BloomExtract, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/BloomExtract.frag");
    gsLoadShader(Shader_GaussianBlur, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/GaussianBlur.frag");
    gsLoadShader(Shader_BloomCombine, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/BloomCombine.frag");
    gsLoadShader(Shader_FXAA, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/FXAA.frag");

    // �p�����[�^���Z�b�g
    is_avoid_effect_ = false;
}

void PostEffect::start() const {
    gsBeginRenderTarget(Rt_Base);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostEffect::end() const {
    gsEndRenderTarget();
}

void PostEffect::draw() const {
    GSuint current = Rt_Base;

    // ������o��������
    if (is_avoid_effect_) current = apply_avoid_effect(current, Rt_AvoidMask);

    // �|�X�g�G�t�F�N�g��������
    if (setting_.is_draw_posteffect()) {
        // �P�x�̍������������o��
        bloom_extract(current);

        // �K�E�V�A���u���[�łڂ���
        gaussian_blur(Rt_BloomExtract, { width_ / 4.0f,  height_ / 4.0f }, Rt_GaussianBlurH1, Rt_GaussianBlurV1);
        gaussian_blur(Rt_GaussianBlurV1, { width_ / 8.0f,  height_ / 8.0f }, Rt_GaussianBlurH2, Rt_GaussianBlurV2);
        gaussian_blur(Rt_GaussianBlurV2, { width_ / 16.0f, height_ / 16.0f }, Rt_GaussianBlurH3, Rt_GaussianBlurV3);
        gaussian_blur(Rt_GaussianBlurV3, { width_ / 32.0f, height_ / 32.0f }, Rt_GaussianBlurH4, Rt_GaussianBlurV4);

        // �u���[���G�t�F�N�g������
        current = bloom_combine(current);

        // �A���`�G�C���A�V���O
        if (setting_.is_draw_fxaa()) current = apply_fxaa(current);
    }
    // ������̌��ʂ�\��
    gsBindRenderTargetTextureEx(current, 0, 0);
    // �����_�[�^�[�Q�b�g��`��
    gsDrawRenderTarget(current);
}

void PostEffect::start_avoid_effect() const {
    // �}�X�N�����̂��߂ɐ[�x���ė��p����
    GStexture* texture = gsGetRenderTargetDepthTexture(Rt_Base);
    GSuint id = texture->dwTexName;
    
    gsBeginRenderTarget(Rt_AvoidMask);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT, // �����ւ�
        GL_TEXTURE_2D,
        id,
        0
    );

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);  // �[�x�ւ̏������݂��֎~
}

void PostEffect::end_avoid_effect() const {
    glDepthMask(GL_TRUE);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        0,                   // ����
        0
    );

    gsEndRenderTarget();
}

bool& PostEffect::enable_avoid_effect() {
    return is_avoid_effect_;
}

bool PostEffect::is_draw_avoid_effect() const {
    return is_avoid_effect_;
}

void PostEffect::set_avoid_color(const GSvector3& color) {
    avoid_color_ = color;
}

GSuint PostEffect::apply_avoid_effect(GSuint source, GSuint mask) const {
    // ������o�p�V�F�[�_�[��L���ɂ���
    gsBeginShader(Shader_AvoidEffect);
    // �e�N�X�`���̐ݒ�
    gsSetShaderParamTexture("u_SceneTexture", 0);
    gsSetShaderParamTexture("u_MaskTexture", 1);
    gsSetShaderParam3f("u_Color", &avoid_color_);  // 0.6f, 0.3f, 1.0f
    // ������o�p�����_�[�^�[�Q�b�g��`���ɂ���
    gsBeginRenderTarget(Rt_AvoidEffect);
    // ������o�p�̃e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(source, 0, 0);
    gsBindRenderTargetTextureEx(mask, 0, 1);
    // �����_�[�^�[�Q�b�g�̕`��
    gsDrawRenderTargetEx(Rt_AvoidEffect);
    // ������o�p�̃e�N�X�`���̃o�C���h����
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    gsUnbindRenderTargetTextureEx(mask, 0, 1);
    // �����_�[�^�[�Q�b�g�̉���
    gsEndRenderTarget();
    // �V�F�[�_�[�𖳌��ɂ���
    gsEndShader();
    return Rt_AvoidEffect;
}

void PostEffect::gaussian_blur(GSuint source, GSvector2 size, GSuint blur_h, GSuint blur_v) const {
    // �u���[�e�N�X�`���̃e�N�Z���T�C�Y
    GSvector2 blur_texel_size{ 1.0f / size.x, 1.0f / size.y };
    // �����u���[�̕���
    GSvector2 blur_h_direction{ 1.0f, 0.0f };
    // �����u���[�̕���
    GSvector2 blur_v_direction{ 0.0f, 1.0f };
    // �K�E�V�A���u���[�p�V�F�[�_�[��L���ɂ���
    gsBeginShader(Shader_GaussianBlur);
    // �e�N�Z���T�C�Y�̐ݒ�
    gsSetShaderParam2f("u_TexelSize", &blur_texel_size);
    // �u���[�̕����𐅕������ɂ���
    gsSetShaderParam2f("u_Direction", &blur_h_direction);
    // �e�N�X�`���̐ݒ�
    gsSetShaderParamTexture("u_RenderTexture", 0);
    // ���������u���[�p�̃����_�[�^�[�Q�b�g��`���ɂ���
    gsBeginRenderTarget(blur_h);
    // ���摜�̃e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(source, 0, 0);
    // �����_�[�^�[�Q�b�g��`��
    gsDrawRenderTargetEx(blur_h);
    // �e�N�X�`���̉���
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // �����_�[�^�[�Q�b�g�̉���
    gsEndRenderTarget();

    // �u���[�̕����𐂒������ɂ���
    gsSetShaderParam2f("u_Direction", &blur_v_direction);
    // ���������u���[�p�̃����_�[�^�[�Q�b�g��`���ɂ���
    gsBeginRenderTarget(blur_v);
    // ���������u���[�̃e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(blur_h, 0, 0);
    // �����_�[�^�[�Q�b�g��`��
    gsDrawRenderTargetEx(blur_v);
    // �e�N�X�`���̉���
    gsUnbindRenderTargetTextureEx(blur_h, 0, 0);
    // �����_�[�^�[�Q�b�g�̉���
    gsEndRenderTarget();
    // �V�F�[�_�[�𖳌��ɂ���
    gsEndShader();
}

GSuint PostEffect::bloom_extract(GSuint source) const {
    // ���P�x�s�N�Z�����o�V�F�[�_�[��L���ɂ���
    gsBeginShader(Shader_BloomExtract);
    // �u���[���̑Ώۂɂ���s�N�Z���̋P�x��ݒ�
    gsSetShaderParam1f("u_BloomThreshold", BLOOM_THRESHOLD);
    // �e�N�X�`���̐ݒ�
    gsSetShaderParamTexture("u_RenderTexture", 0);
    // �P�x���o�p�̃����_�[�^�[�Q�b�g���o�C���h����
    gsBeginRenderTarget(Rt_BloomExtract);
    // ���V�[���摜�p�̃e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(source, 0, 0);
    // �����_�[�^�[�Q�b�g�̕`��
    gsDrawRenderTargetEx(Rt_BloomExtract);
    // ���V�[���摜�p�̃e�N�X�`���̃o�C���h����
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // �����_�[�^�[�Q�b�g�̉���
    gsEndRenderTarget();
    // �V�F�[�_�[�𖳌��ɂ���
    gsEndShader();
    return Rt_BloomExtract;
}

GSuint PostEffect::bloom_combine(GSuint source) const {
    // �u���[�������p�V�F�[�_�[��L���ɂ���
    gsBeginShader(Shader_BloomCombine);
    gsSetShaderParamTexture("u_BaseTexture", 0);
    gsSetShaderParamTexture("u_BlurTexture1", 1);
    gsSetShaderParamTexture("u_BlurTexture2", 2);
    gsSetShaderParamTexture("u_BlurTexture3", 3);
    gsSetShaderParamTexture("u_BlurTexture4", 4);
    gsSetShaderParam1f("u_BloomIntensity", BLOOM_INTENSITY);
    // �u���[�������p�����_�[�^�[�Q�b�g��`���ɂ���
    gsBeginRenderTarget(Rt_BloomCombine);
    // ���V�[���̃e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(source, 0, 0);
    // �u���[�e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV1, 0, 1);
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV2, 0, 2);
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV3, 0, 3);
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV4, 0, 4);
    // �u���[�������p�����_�[�^�[�Q�b�g��`��
    gsDrawRenderTargetEx(Rt_BloomCombine);
    // ���V�[���̃e�N�X�`�����o�C���h����
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // �u���[�e�N�X�`�����o�C���h����
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV1, 0, 1);
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV2, 0, 2);
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV3, 0, 3);
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV4, 0, 4);
    // �����_�[�^�[�Q�b�g�̉����@
    gsEndRenderTarget();
    // �V�F�[�_�[�𖳌��ɂ���
    gsEndShader();
    return Rt_BloomCombine;
}

GSuint PostEffect::apply_fxaa(GSuint source) const {
    // �X�N���[���T�C�Y�̋t��
    GSvector2 inv_screen_size{ 1.0f / (float)width_, 1.0f / (float)height_ };
    // �A���`�G�C���A�V���O�p�V�F�[�_�[��L���ɂ���
    gsBeginShader(Shader_FXAA);
    // �e�N�X�`���̐ݒ�
    gsSetShaderParamTexture("u_SceneTexture", 0);
    // �X�N���[���T�C�Y�̋t����ݒ�
    gsSetShaderParam2f("u_InvScreenSize", &inv_screen_size);
    // �A���`�G�C���A�V���O�p�����_�[�^�[�Q�b�g��`���ɂ���
    gsBeginRenderTarget(Rt_FXAA);
    // �e�N�X�`�����o�C���h
    gsBindRenderTargetTextureEx(source, 0, 0);
    // �����_�[�^�[�Q�b�g�̕`��
    gsDrawRenderTargetEx(Rt_FXAA);
    // �e�N�X�`���̃o�C���h����
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // �����_�[�^�[�Q�b�g�̉����@
    gsEndRenderTarget();
    // �V�F�[�_�[�𖳌��ɂ���
    gsEndShader();
    return Rt_FXAA;
}
