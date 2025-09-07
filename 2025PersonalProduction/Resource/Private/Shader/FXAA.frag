#version 330

layout(location = 0) out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

// 最終テクスチャ
uniform sampler2D u_SceneTexture;
// スクリーンサイズの逆数(例: w = 1.0/1920.0, h = 1.0/1080.0)
uniform vec2 u_InvScreenSize;

// メインの探索ステップ数 (品質に直結)
#define FXAA_QUALITY_PS 8
// エッジ検出の感度（高いほど弱いエッジも検出）
#define FXAA_QUALITY_EDGE_THRESHOLD (1.0/8.0) 
// 検出する最小の輝度差
#define FXAA_QUALITY_EDGE_THRESHOLD_MIN (1.0/32.0) 

// 輝度への変換係数
const vec3 LUMA_VEC = vec3(0.299, 0.587, 0.114);

vec4 FxaaPixelShader(vec2 texCoord) {
    // 周辺ピクセルの輝度を取得
    // 中心、北、南、東、西のテクスチャ座標を計算
    vec2 posM = texCoord;
    vec2 posN = posM + vec2(0.0,  u_InvScreenSize.y);
    vec2 posS = posM - vec2(0.0,  u_InvScreenSize.y);
    vec2 posE = posM + vec2(u_InvScreenSize.x, 0.0);
    vec2 posW = posM - vec2(u_InvScreenSize.x, 0.0);

    // テクスチャをサンプリングしてRGBを取得し、輝度に変換
    float lumaM = dot(texture(u_SceneTexture, posM).rgb, LUMA_VEC);
    float lumaN = dot(texture(u_SceneTexture, posN).rgb, LUMA_VEC);
    float lumaS = dot(texture(u_SceneTexture, posS).rgb, LUMA_VEC);
    float lumaE = dot(texture(u_SceneTexture, posE).rgb, LUMA_VEC);
    float lumaW = dot(texture(u_SceneTexture, posW).rgb, LUMA_VEC);

    // ローカルコントラストを計算し、エッジか判定
    float lumaMin = min(lumaM, min(min(lumaN, lumaS), min(lumaE, lumaW)));
    float lumaMax = max(lumaM, max(max(lumaN, lumaS), max(lumaE, lumaW)));
    float lumaRange = lumaMax - lumaMin;

    // 輝度差が小さすぎる場合は、AA処理をせず元の色を返して終了
    if (lumaRange < max(FXAA_QUALITY_EDGE_THRESHOLD_MIN, lumaMax * FXAA_QUALITY_EDGE_THRESHOLD)) {
        return texture(u_SceneTexture, posM);   // ボケ防止
    }
    
    // コーナーピクセルもサンプリング
    vec2 posNW = posM + vec2(-u_InvScreenSize.x,  u_InvScreenSize.y);
    vec2 posNE = posM + vec2( u_InvScreenSize.x,  u_InvScreenSize.y);
    vec2 posSW = posM + vec2(-u_InvScreenSize.x, -u_InvScreenSize.y);
    vec2 posSE = posM + vec2( u_InvScreenSize.x, -u_InvScreenSize.y);

    float lumaNW = dot(texture(u_SceneTexture, posNW).rgb, LUMA_VEC);
    float lumaNE = dot(texture(u_SceneTexture, posNE).rgb, LUMA_VEC);
    float lumaSW = dot(texture(u_SceneTexture, posSW).rgb, LUMA_VEC);
    float lumaSE = dot(texture(u_SceneTexture, posSE).rgb, LUMA_VEC);

    // エッジの方向を判定
    float edgeVert = abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW) + 
                       (0.50 * lumaN ) + (-1.0 * lumaM) + (0.50 * lumaS ) +
                       (0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    float edgeHorz = abs((0.25 * lumaNW) + ( 0.5 * lumaN) + (0.25 * lumaNE) + 
                       (-0.5 * lumaW ) + (-1.0 * lumaM) + (-0.5 * lumaE ) +
                       (0.25 * lumaSW) + ( 0.5 * lumaS) + (0.25 * lumaSE));
    bool isHorizontal = (edgeHorz >= edgeVert);
    
    // エッジの探索方向と探索ステップ幅を決定
    float stepLength = isHorizontal ? u_InvScreenSize.y : u_InvScreenSize.x;
    vec2 stepDir;
    if (isHorizontal)  stepDir = vec2(0.0, stepLength); 
    else stepDir = vec2(stepLength, 0.0); 

    // エッジの両端（プラス方向とマイナス方向）を探索する
    // 現在位置の2辺の輝度の平均を計算 (エッジ探索の基準値)
    float lumaEdgeAverage;
    if (isHorizontal) lumaEdgeAverage = (lumaN + lumaS) * 0.5; 
    else lumaEdgeAverage = (lumaE + lumaW) * 0.5;
    
    // プラス方向へ探索
    vec2 posP = posM;
    vec2 offP = vec2(0.0);

    if (isHorizontal)offP.y = u_InvScreenSize.y; 
    else offP.x = u_InvScreenSize.x; 

    float lumaEndP = lumaM;
    for (int i = 0; i < FXAA_QUALITY_PS; i++) {
        posP += offP;
        lumaEndP = dot(texture(u_SceneTexture, posP).rgb, LUMA_VEC);
        // 平均輝度との差が閾値を超えたら、エッジの端と判定
        if(abs(lumaEndP - lumaEdgeAverage) >= lumaRange * 0.5) break; 
    }

    // マイナス方向へ探索
    vec2 posN_ = posM;
    vec2 offN = -offP;
    float lumaEndN = lumaM;
    for (int i = 0; i < FXAA_QUALITY_PS; i++) {
        posN_ += offN;
        lumaEndN = dot(texture(u_SceneTexture, posN_).rgb, LUMA_VEC);
        // 平均輝度との差が閾値を超えたら、エッジの端と判定
        if (abs(lumaEndN - lumaEdgeAverage) >= lumaRange * 0.5) break;
    }
    
    // エッジの中心位置（サブピクセル）を計算
    float distP = isHorizontal ? (posP.y - posM.y) : (posP.x - posM.x);
    float distN = isHorizontal ? (posM.y - posN_.y) : (posM.x - posN_.x);
    
    float pixelOffset = 0.0;
    if (distP < distN) pixelOffset = -stepLength * 0.5 * (distP / (distP + distN));
    else  pixelOffset = stepLength * 0.5 * (distN / (distP + distN));

    // サブピクセルオフセットが計算できたら、それをUVに適用
    vec2 uvOffset = isHorizontal ? vec2(0.0, pixelOffset) : vec2(pixelOffset, 0.0);
    vec2 finalUV = posM + uvOffset;

    // 計算したサブピクセル座標から最終的な色をサンプリング
    return texture(u_SceneTexture, finalUV);
}

void main() {
    // 最終カラーの出力
    out_FragColor = FxaaPixelShader(v_TexCoord);
}