#version 330

layout(location = 0) out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

uniform sampler2D u_SceneTexture; // シーンテクスチャ
uniform sampler2D u_AOTexture;    // ブラー後のSSAOマップ

void main() {
    // シーンの元の色を取得
    vec3 sceneColor = texture(u_SceneTexture, v_TexCoord).rgb;
    // AOマップから遮蔽率を取得
    float ao = texture(u_AOTexture, v_TexCoord).r; 
    
    // シーンカラーに遮蔽率を乗算して、影の部分を暗くする
    out_FragColor = vec4(sceneColor * ao, 1.0);
}
