#version 330
layout(location = 0) out vec4 out_FragColor;

// テクスチャ座標
in vec2 v_TexCoord;

// ベースレンダーテクスチャ 
uniform sampler2D u_RenderTexture;
// デプスバッファ
uniform sampler2D u_DepthTexture;

// 強さ
uniform float u_Power;
// アスペクト比
uniform float u_AspectRatio;
// インパクト位置
uniform vec2 u_ImpactPosition;

const float MASK_INNER_START = 0.1; // ここまで線なし
const float MASK_INNER_END   = 0.2; // ここから線あり
const float MASK_OUTER_START = 0.6;  // ここまで線あり
const float MASK_OUTER_END   = 0.8;  // ここから線なし

const float LENGTH_RANDOMNESS = 0.5;    // 外側の振れ幅

// 集中線の密度
const float LINE_DENSITY = 150.0;
// 線の太さと線の鋭さ
const float LINE_SHARPNESS = 0.8;
// 背景を白黒どちらに倒すかの境界線
const float BG_THRESHOLD = 0.1;
// 線の色を白黒どちらにするかの輝度閾値
const float COLOR_THRESHOLD = 0.07;
// 集中線の取得位置
const float COLOR_SAMPLE_OFFSET = 0.1;

const float NEAR = 0.3; 
const float FAR = 1000;

// FOGの開始地点
const float FOG_START_DIST = 5.0; 
// FOGの完了地点
const float FOG_END_DIST   = 6.0;

// 最終色
const vec3 COLOR_DARK  = vec3(0.15, 0.15, 0.15); 
const vec3 COLOR_LIGHT = vec3(0.9, 0.9, 0.9);

float hash12(vec2 p) {
	vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

float angularNoise(float angle) {
    float val = angle * LINE_DENSITY;
    float i = floor(val);
    float f = fract(val);
    float n1 = hash12(vec2(sin(i / LINE_DENSITY), 1.0));
    float n2 = hash12(vec2(sin((i + 1.0) / LINE_DENSITY), 1.0));
    return mix(n1, n2, smoothstep(0.0, 1.0, f));
}

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main() {
    // 元のシーンの色と輝度を取得
    vec4 baseColor = texture(u_RenderTexture, v_TexCoord);
    float baseGray = dot(baseColor.rgb, vec3(0.299, 0.587, 0.114));
    float baseBinary = step(BG_THRESHOLD, baseGray);

    // 現在のピクセルからインパクト位置へのベクトル
    vec2 pos = v_TexCoord - u_ImpactPosition;
    // アスペクト比補正
    pos.x *= u_AspectRatio;

    float binaryBgVal = step(BG_THRESHOLD, baseGray);
    vec3 bgColor = vec3(binaryBgVal);
    
    float dist = length(pos);           // インパクト位置からの距離
    float angle = atan(pos.y, pos.x);   // インパクト位置からの角度

    // ノイズ取得
    float noiseVal = angularNoise(angle);
    
    // 線自体の輪郭もパキッとさせる
    float rawLinePattern = step(LINE_SHARPNESS, noiseVal);

    // 内側
    float centerMask = smoothstep(MASK_INNER_START, MASK_INNER_END, dist);
    
    // ランダムな長さを計算
    float lenNoise = angularNoise(angle + 12.34); 
    float randomOffset = lenNoise * LENGTH_RANDOMNESS;

    float currentStart = MASK_OUTER_START + randomOffset;
    float currentEnd   = MASK_OUTER_END   + randomOffset;

    // 外側
    float outerMask = smoothstep(currentEnd, currentStart, dist);
    float isOuterArea = 1.0 - step(0.5, outerMask);

    float areaMask = centerMask * outerMask;
    
    // 線の描画有無
    float lineActive = rawLinePattern * areaMask;

    // 集中線の場合内側の色を取得する
    vec2 vecFromCenter = v_TexCoord - u_ImpactPosition;
    vec2 sampleUV = u_ImpactPosition + vecFromCenter * (1.0 - COLOR_SAMPLE_OFFSET);
    vec4 offsetColor = texture(u_RenderTexture, sampleUV);
    float offsetGray = dot(offsetColor.rgb, vec3(0.299, 0.587, 0.114));
    float offsetBinary = step(COLOR_THRESHOLD, offsetGray);

    // 色を確定
    float isLineWhite = step(COLOR_THRESHOLD, offsetGray);
    vec3 targetLineColor = vec3(isLineWhite);

    // 合成
    vec3 finalBinaryColor = mix(bgColor, targetLineColor, lineActive);
    vec3 softColor = mix(COLOR_DARK, COLOR_LIGHT, finalBinaryColor.r);

    // 深度を取得
    float rawDepth = texture(u_DepthTexture, v_TexCoord).r;
    float linearDist = LinearizeDepth(rawDepth);

    // FOG
    float fog = step(FOG_END_DIST, linearDist);
    // 白で塗りつぶす
    softColor = mix(softColor, COLOR_LIGHT, fog);

    out_FragColor = vec4(mix(baseColor.rgb, softColor, u_Power), 1.0);
}
