#version 330

layout(location = 0) out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

// 元画像のテクスチャ
uniform sampler2D u_RenderTexture;
// デプスバッファ
uniform sampler2D u_DepthTexture;

// フォグのカラー
uniform vec4 u_FogColor;
// フォグの開始位置（視点座標系）
uniform float u_FogStart;
// フォグの終了位置（視点座標系）
uniform float u_FogEnd;

// zバッファの値を線形に変換するパラメータ
uniform vec4 u_ZBufferParams;

// デプスバッファの値から視点からの距離を求める
float LinearEyeDepth(float z) {
    return 1.0 / (u_ZBufferParams.z * z + u_ZBufferParams.w);
}

void main(void) {
    // デプスバッファの値を視点からの距離に変換する
    float depth = LinearEyeDepth(texture(u_DepthTexture, v_TexCoord).r);
    // フォグの開始位置・フォグの終了位置・デプスバッファの値からフォグの強さを求める
    float fog = smoothstep(u_FogStart, u_FogEnd, depth);
    // メインカラーを取得
    vec4 mainColor = texture(u_RenderTexture, v_TexCoord);
    // メインカラーとフォグのカラーを線形補間
    vec4 col = mix(mainColor, u_FogColor, fog);
    // 最終カラーの出力
    out_FragColor = vec4(col.rgb, 1.0);
}


