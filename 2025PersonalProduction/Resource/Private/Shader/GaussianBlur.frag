#version 330
layout(location = 0) out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

// ブラーの方向 横方向(1.0, 0.0), 縦方向(0.0, 1.0)
uniform vec2 u_Direction;
// テクスチャ座標での１テクセルのサイズ
uniform vec2 u_TexelSize;

// レンダーターゲット
uniform sampler2D u_RenderTexture;

void main(void) {
    // ガウシアンブラーの重み
    const float[5] weights = float[](
      0.17158221, 0.15839034, 0.12459426, 0.08351803, 0.04770625
    );
    // 左右（または上下）の9ピクセルの加重平均値を求める
    vec4 col = texture(u_RenderTexture, v_TexCoord) * weights[0];
    for (int i = 1; i < 5; ++i) {
        vec2 offset = u_TexelSize * i * u_Direction;
        col += texture(u_RenderTexture, v_TexCoord + offset) * weights[i];
        col += texture(u_RenderTexture, v_TexCoord - offset) * weights[i];
    }
    out_FragColor = vec4(col.rgb, 1.0f);
}
