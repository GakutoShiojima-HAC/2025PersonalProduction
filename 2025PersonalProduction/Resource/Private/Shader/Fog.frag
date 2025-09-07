#version 330

layout(location = 0) out vec4 out_FragColor;
// �e�N�X�`�����W
in vec2 v_TexCoord;

// ���摜�̃e�N�X�`��
uniform sampler2D u_RenderTexture;
// �f�v�X�o�b�t�@
uniform sampler2D u_DepthTexture;

// �t�H�O�̃J���[
uniform vec4 u_FogColor;
// �t�H�O�̊J�n�ʒu�i���_���W�n�j
uniform float u_FogStart;
// �t�H�O�̏I���ʒu�i���_���W�n�j
uniform float u_FogEnd;

// z�o�b�t�@�̒l����`�ɕϊ�����p�����[�^
uniform vec4 u_ZBufferParams;

// �f�v�X�o�b�t�@�̒l���王�_����̋��������߂�
float LinearEyeDepth(float z) {
    return 1.0 / (u_ZBufferParams.z * z + u_ZBufferParams.w);
}

void main(void) {
    // �f�v�X�o�b�t�@�̒l�����_����̋����ɕϊ�����
    float depth = LinearEyeDepth(texture(u_DepthTexture, v_TexCoord).r);
    // �t�H�O�̊J�n�ʒu�E�t�H�O�̏I���ʒu�E�f�v�X�o�b�t�@�̒l����t�H�O�̋��������߂�
    float fog = smoothstep(u_FogStart, u_FogEnd, depth);
    // ���C���J���[���擾
    vec4 mainColor = texture(u_RenderTexture, v_TexCoord);
    // ���C���J���[�ƃt�H�O�̃J���[����`���
    vec4 col = mix(mainColor, u_FogColor, fog);
    // �ŏI�J���[�̏o��
    out_FragColor = vec4(col.rgb, 1.0);
}


