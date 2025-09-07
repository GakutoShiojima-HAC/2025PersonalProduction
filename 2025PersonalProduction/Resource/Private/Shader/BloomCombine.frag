#version 330
layout(location = 0) out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

// シーンのテクスチャ
uniform sampler2D u_BaseTexture;
// ブラーテクスチャ
uniform sampler2D u_BlurTexture1;
uniform sampler2D u_BlurTexture2;
uniform sampler2D u_BlurTexture3;
uniform sampler2D u_BlurTexture4;
// ブルーム効果の強さ
uniform float u_BloomIntensity = 1.0f;

// トーンマッピング
vec3 ACESFilm(vec3 x) {
    x *= 0.6;
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

// ガンマカラー空間からリニアカラー空間に変換
vec3 GammaToLinearSpace(vec3 color) {
    return pow(color, vec3(2.2));
}

// リニアカラー空間からガンマカラー空間に変換
vec3 LinearToGammaSpace(vec3 color) {
    return pow(color, vec3(1.0/2.2));
}

void main(void) {
   // シーンのカラーを取得
    vec4 col = texture(u_BaseTexture, v_TexCoord);
    // リニアカラー空間に変換
    col.rgb = GammaToLinearSpace(col.rgb);
    // ブルームのカラーを合成
    vec4 bloom = vec4(0.0);
    bloom += texture(u_BlurTexture1, v_TexCoord);
    bloom += texture(u_BlurTexture2, v_TexCoord);
    bloom += texture(u_BlurTexture3, v_TexCoord);
    bloom += texture(u_BlurTexture4, v_TexCoord);
    // シーンのカラーとブルームのカラーを合成
    col += bloom * u_BloomIntensity;
     // トーンマッピング
    col.rgb = ACESFilm(col.rgb);
    // ガンマカラー空間に変換
    col.rgb = LinearToGammaSpace(col.rgb);
    out_FragColor = vec4(col.rgb, 1.0);
}