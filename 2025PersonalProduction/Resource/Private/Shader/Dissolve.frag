#version 330
layout(location = 0) out vec4 out_FragColor;

// テクスチャ座標
in vec2 v_TexCoord;

// ベースレンダーテクスチャ 
uniform sampler2D u_RenderTexture;
// マスク用テクスチャ
uniform sampler2D u_MaskTexture;
// 背景用テクスチャ
uniform sampler2D u_BackGroundTexture;
// UV座標のタイリング
uniform vec2 u_Tilling;
// UV座標のオフセット
uniform vec2 u_Offset;
// ディゾルブするしきい値
uniform float u_Threshold;
// エッジの幅
uniform float u_EdgeWidth;
// エッジのカラー
uniform vec4 u_EdgeColor;
// エッジの輝度
uniform float u_EdgeColorIntensity;

void main(void) {
    // ベースのテクスチャを取得
    vec4 baseColor = texture(u_RenderTexture, v_TexCoord);
    // 背景のテクスチャを取得
    vec2 bgUV = vec2(v_TexCoord.x, 1.0 - v_TexCoord.y);
    vec4 bgColor = texture(u_BackGroundTexture, bgUV);

    // マスク用テクスチャのタイリングとオフセットを加える
    vec2 uv = v_TexCoord * vec2(1.0, -1.0) * u_Tilling + u_Offset;
    float mask = texture(u_MaskTexture, uv).r;
    // エッジの抽出
    float edge = smoothstep(u_Threshold, u_Threshold * (1.0 + u_EdgeWidth), mask);
    // エッジのカラーを加算
    baseColor += u_EdgeColor * edge * u_EdgeColorIntensity;
    // 消失する範囲を求める(maskがu_Treasholdよりも小さければ０になる）
    float cutoff = step(mask, u_Threshold * (1.0 + u_EdgeWidth));
    // 消失する部分は背景画像を使う
    vec4 finalColor = mix(bgColor, baseColor, cutoff);
    // 最終カラーの出力
    out_FragColor = vec4(finalColor.rgb, 1.0);
}
