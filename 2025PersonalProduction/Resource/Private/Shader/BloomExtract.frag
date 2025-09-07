#version 330
layout(location = 0) out vec4 out_FragColor;

// テクスチャ座標
in vec2 v_TexCoord;

// レンダーテクスチャ 
uniform sampler2D u_RenderTexture;
// 輝度のしきい値
uniform float u_BloomThreshold = 1.0f;

// ガンマカラー空間からリニアカラー空間に変換
vec3 GammaToLinearSpace(vec3 color) {
    return pow(color, vec3(2.2));
}

void main(void) {
    // レンダーテクスチャのカラーを取得
    vec4 color = texture(u_RenderTexture, v_TexCoord);
     // リニアカラー空間に変換
    color.rgb = GammaToLinearSpace(color.rgb);
    // テクセルの輝度を求める
    float luminance = color.r * 0.33 + color.g * 0.34 + color.b * 0.33;
    // 指定した輝度以上のピクセルだけ残す
    luminance = max(0.0, luminance - u_BloomThreshold);
    // signはプラスの値の場合は1.0,ゼロの場合は0.0となるので、しきい値以下は黒になる
    color.rgb *= sign(luminance);
    // 最終カラーの出力
    out_FragColor = vec4(color.rgb, 1.0);
}