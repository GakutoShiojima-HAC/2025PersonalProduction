// �����_�[�e�N�X�`���p���_�V�F�[�_�[
#version 330
layout(location = 0)  in vec4 in_Position;  // ���_���W
layout(location = 8)  in vec2 in_TexCoord;  // �e�N�X�`�����W

// �t���O�����g�V�F�[�_�ɏo�͂���e�N�X�`�����W
out vec2 v_TexCoord;

void main(void) {
    // �e�N�X�`�����W���t���O�����g�V�F�[�_�ɂ��̂܂܏o��
    v_TexCoord  = in_TexCoord;
    // ���_���W�����̂܂܏o��
    gl_Position = in_Position;
}