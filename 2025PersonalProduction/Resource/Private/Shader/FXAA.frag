#version 330

layout(location = 0) out vec4 out_FragColor;
// �e�N�X�`�����W
in vec2 v_TexCoord;

// �ŏI�e�N�X�`��
uniform sampler2D u_SceneTexture;
// �X�N���[���T�C�Y�̋t��(��: w = 1.0/1920.0, h = 1.0/1080.0)
uniform vec2 u_InvScreenSize;

// ���C���̒T���X�e�b�v�� (�i���ɒ���)
#define FXAA_QUALITY_PS 8
// �G�b�W���o�̊��x�i�����قǎア�G�b�W�����o�j
#define FXAA_QUALITY_EDGE_THRESHOLD (1.0/8.0) 
// ���o����ŏ��̋P�x��
#define FXAA_QUALITY_EDGE_THRESHOLD_MIN (1.0/32.0) 

// �P�x�ւ̕ϊ��W��
const vec3 LUMA_VEC = vec3(0.299, 0.587, 0.114);

vec4 FxaaPixelShader(vec2 texCoord) {
    // ���Ӄs�N�Z���̋P�x���擾
    // ���S�A�k�A��A���A���̃e�N�X�`�����W���v�Z
    vec2 posM = texCoord;
    vec2 posN = posM + vec2(0.0,  u_InvScreenSize.y);
    vec2 posS = posM - vec2(0.0,  u_InvScreenSize.y);
    vec2 posE = posM + vec2(u_InvScreenSize.x, 0.0);
    vec2 posW = posM - vec2(u_InvScreenSize.x, 0.0);

    // �e�N�X�`�����T���v�����O����RGB���擾���A�P�x�ɕϊ�
    float lumaM = dot(texture(u_SceneTexture, posM).rgb, LUMA_VEC);
    float lumaN = dot(texture(u_SceneTexture, posN).rgb, LUMA_VEC);
    float lumaS = dot(texture(u_SceneTexture, posS).rgb, LUMA_VEC);
    float lumaE = dot(texture(u_SceneTexture, posE).rgb, LUMA_VEC);
    float lumaW = dot(texture(u_SceneTexture, posW).rgb, LUMA_VEC);

    // ���[�J���R���g���X�g���v�Z���A�G�b�W������
    float lumaMin = min(lumaM, min(min(lumaN, lumaS), min(lumaE, lumaW)));
    float lumaMax = max(lumaM, max(max(lumaN, lumaS), max(lumaE, lumaW)));
    float lumaRange = lumaMax - lumaMin;

    // �P�x��������������ꍇ�́AAA�������������̐F��Ԃ��ďI��
    if (lumaRange < max(FXAA_QUALITY_EDGE_THRESHOLD_MIN, lumaMax * FXAA_QUALITY_EDGE_THRESHOLD)) {
        return texture(u_SceneTexture, posM);   // �{�P�h�~
    }
    
    // �R�[�i�[�s�N�Z�����T���v�����O
    vec2 posNW = posM + vec2(-u_InvScreenSize.x,  u_InvScreenSize.y);
    vec2 posNE = posM + vec2( u_InvScreenSize.x,  u_InvScreenSize.y);
    vec2 posSW = posM + vec2(-u_InvScreenSize.x, -u_InvScreenSize.y);
    vec2 posSE = posM + vec2( u_InvScreenSize.x, -u_InvScreenSize.y);

    float lumaNW = dot(texture(u_SceneTexture, posNW).rgb, LUMA_VEC);
    float lumaNE = dot(texture(u_SceneTexture, posNE).rgb, LUMA_VEC);
    float lumaSW = dot(texture(u_SceneTexture, posSW).rgb, LUMA_VEC);
    float lumaSE = dot(texture(u_SceneTexture, posSE).rgb, LUMA_VEC);

    // �G�b�W�̕����𔻒�
    float edgeVert = abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW) + 
                       (0.50 * lumaN ) + (-1.0 * lumaM) + (0.50 * lumaS ) +
                       (0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    float edgeHorz = abs((0.25 * lumaNW) + ( 0.5 * lumaN) + (0.25 * lumaNE) + 
                       (-0.5 * lumaW ) + (-1.0 * lumaM) + (-0.5 * lumaE ) +
                       (0.25 * lumaSW) + ( 0.5 * lumaS) + (0.25 * lumaSE));
    bool isHorizontal = (edgeHorz >= edgeVert);
    
    // �G�b�W�̒T�������ƒT���X�e�b�v��������
    float stepLength = isHorizontal ? u_InvScreenSize.y : u_InvScreenSize.x;
    vec2 stepDir;
    if (isHorizontal)  stepDir = vec2(0.0, stepLength); 
    else stepDir = vec2(stepLength, 0.0); 

    // �G�b�W�̗��[�i�v���X�����ƃ}�C�i�X�����j��T������
    // ���݈ʒu��2�ӂ̋P�x�̕��ς��v�Z (�G�b�W�T���̊�l)
    float lumaEdgeAverage;
    if (isHorizontal) lumaEdgeAverage = (lumaN + lumaS) * 0.5; 
    else lumaEdgeAverage = (lumaE + lumaW) * 0.5;
    
    // �v���X�����֒T��
    vec2 posP = posM;
    vec2 offP = vec2(0.0);

    if (isHorizontal)offP.y = u_InvScreenSize.y; 
    else offP.x = u_InvScreenSize.x; 

    float lumaEndP = lumaM;
    for (int i = 0; i < FXAA_QUALITY_PS; i++) {
        posP += offP;
        lumaEndP = dot(texture(u_SceneTexture, posP).rgb, LUMA_VEC);
        // ���ϋP�x�Ƃ̍���臒l�𒴂�����A�G�b�W�̒[�Ɣ���
        if(abs(lumaEndP - lumaEdgeAverage) >= lumaRange * 0.5) break; 
    }

    // �}�C�i�X�����֒T��
    vec2 posN_ = posM;
    vec2 offN = -offP;
    float lumaEndN = lumaM;
    for (int i = 0; i < FXAA_QUALITY_PS; i++) {
        posN_ += offN;
        lumaEndN = dot(texture(u_SceneTexture, posN_).rgb, LUMA_VEC);
        // ���ϋP�x�Ƃ̍���臒l�𒴂�����A�G�b�W�̒[�Ɣ���
        if (abs(lumaEndN - lumaEdgeAverage) >= lumaRange * 0.5) break;
    }
    
    // �G�b�W�̒��S�ʒu�i�T�u�s�N�Z���j���v�Z
    float distP = isHorizontal ? (posP.y - posM.y) : (posP.x - posM.x);
    float distN = isHorizontal ? (posM.y - posN_.y) : (posM.x - posN_.x);
    
    float pixelOffset = 0.0;
    if (distP < distN) pixelOffset = -stepLength * 0.5 * (distP / (distP + distN));
    else  pixelOffset = stepLength * 0.5 * (distN / (distP + distN));

    // �T�u�s�N�Z���I�t�Z�b�g���v�Z�ł�����A�����UV�ɓK�p
    vec2 uvOffset = isHorizontal ? vec2(0.0, pixelOffset) : vec2(pixelOffset, 0.0);
    vec2 finalUV = posM + uvOffset;

    // �v�Z�����T�u�s�N�Z�����W����ŏI�I�ȐF���T���v�����O
    return texture(u_SceneTexture, finalUV);
}

void main() {
    // �ŏI�J���[�̏o��
    out_FragColor = FxaaPixelShader(v_TexCoord);
}