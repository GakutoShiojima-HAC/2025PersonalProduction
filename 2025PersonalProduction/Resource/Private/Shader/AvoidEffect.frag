#version 330
layout(location = 0) out vec4 out_FragColor;
// �e�N�X�`�����W
in vec2 v_TexCoord;

uniform sampler2D u_SceneTexture; // �V�[��HDR�摜
uniform sampler2D u_MaskTexture;  // �����}�X�N�摜
uniform vec3 u_Color;   // �w�i����߂�F

const vec3 LUMA_VEC = vec3(0.299, 0.587, 0.114);
const float DESATURATION_FACTOR = 0.9;

void main() {
    vec3 sceneColor = texture(u_SceneTexture, v_TexCoord).rgb;
    vec3 maskColor  = texture(u_MaskTexture, v_TexCoord).rgb; // �}�X�N�e�N�X�`������F���擾

    // ���邳�̌v�Z
    float brightness = dot(maskColor, LUMA_VEC);

    // �قڍ��Ȃ�
    if (brightness < 0.01)  {
        // �w�i�̏ꍇ�����A���ɂ��鏈��
        float luma = dot(sceneColor, LUMA_VEC);
        vec3 grayscale = vec3(luma);
        vec3 desaturated = mix(sceneColor, grayscale, DESATURATION_FACTOR);
        vec3 finalColor = desaturated * u_Color;
        out_FragColor = vec4(finalColor, 1.0);
    }
    // 0.5�ȏ� = ��
    else {
        // ���̃V�[���̐F�����̂܂܏o��
        out_FragColor = vec4(sceneColor, 1.0);
    }
}