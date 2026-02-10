#version 330
layout(location = 0) out vec4 out_FragColor;

// テクスチャ座標
in vec2 v_TexCoord;

// ベースレンダーテクスチャ 
uniform sampler2D u_RenderTexture;
// ブラーの強さ
uniform float u_BlurStrength;

// サンプリング回数
const int SAMPLES = 12;
// 画面の中心
const vec2 SCREEN_CENTER = vec2(0.5, 0.5);

void main() {
    // uv座標取得
    vec2 uv = v_TexCoord;
    // 中心へのベクトル
    vec2 dir = SCREEN_CENTER - uv; 
    
    // 距離
    float dist = length(dir);

    // ブラーの強さ
    vec2 velocity = dir * u_BlurStrength * dist;

    vec3 color = vec3(0.0);
    float totalWeight = 0.0;

    // 放射状にサンプリングして加算合成
    for (int i = 0; i < SAMPLES; i++) {
        // 現在のサンプリング位置
        float t = float(i) / float(SAMPLES - 1);
        
        // 中心に向かって少しずつずらす
        vec2 sampleUV = uv + velocity * t;

        // 色ズレの強さ
        float aberration = dist * 0.02 * u_BlurStrength; 

        // RとBを逆方向にずらす
        float r = texture(u_RenderTexture, sampleUV + aberration).r;
        float g = texture(u_RenderTexture, sampleUV).g;
        float b = texture(u_RenderTexture, sampleUV - aberration).b;

        // 重み付け
        float weight = 1.0; 
        
        color += vec3(r, g, b) * weight;
        totalWeight += weight;
    }

    // 平均化
    out_FragColor = vec4(color / totalWeight, 1.0);
}
