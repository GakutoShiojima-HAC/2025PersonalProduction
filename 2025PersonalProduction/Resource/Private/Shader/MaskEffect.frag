#version 330
layout(location = 0) out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

uniform sampler2D u_SceneTexture; // シーンHDR画像
uniform sampler2D u_MaskTexture;  // 白黒マスク画像
uniform vec3 u_Color;   // 背景を染める色

const vec3 LUMA_VEC = vec3(0.299, 0.587, 0.114);
const float DESATURATION_FACTOR = 0.9;

void main() {
    vec3 sceneColor = texture(u_SceneTexture, v_TexCoord).rgb;
    vec3 maskColor  = texture(u_MaskTexture, v_TexCoord).rgb; // マスクテクスチャから色を取得

    // 明るさの計算
    float brightness = dot(maskColor, LUMA_VEC);

    // ほぼ黒なら
    if (brightness < 0.15)  {
        // 背景の場合だけ、紫にする処理
        float luma = dot(sceneColor, LUMA_VEC);
        vec3 grayscale = vec3(luma);
        vec3 desaturated = mix(sceneColor, grayscale, DESATURATION_FACTOR);
        vec3 finalColor = desaturated * u_Color;
        out_FragColor = vec4(finalColor, 1.0);
    }
    else {
        // 元のシーンの色をそのまま出力
        out_FragColor = vec4(sceneColor, 1.0);
    }
}
