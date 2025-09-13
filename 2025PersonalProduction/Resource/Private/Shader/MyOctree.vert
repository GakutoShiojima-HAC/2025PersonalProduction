#version 330
layout(location = 0) in vec4  in_Position;      // 頂点座標
layout(location = 2) in vec3  in_Normal;        // 法線ベクトル
layout(location = 8) in vec2  in_TexCoord;      // テクスチャ座標
layout(location = 9) in vec2  in_TexCoord2;     // テクスチャ座標（ライトマップ用）
layout(location = 14) in vec3 in_Tangent;       // 接ベクトル
layout(location = 15) in vec3 in_Binormal;      // 従法線ベクトル

out vec4 v_WorldPosition;                       // 頂点座標
out vec3 v_WorldNormal;                         // 法線ベクトル
out vec3 v_WorldTangent;                        // 接ベクトル
out vec3 v_WorldBinormal;                       // 従法線ベクトル
out vec2 v_TexCoord;                            // テクスチャ座標
out vec2 v_TexCoord2;                           // ライトマップ用のテクスチャ座標
out vec3 v_ViewNormal;                          // 法線マップ用ビューベクトル

// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;
// モデルビュー変換行列
uniform mat4 gs_ModelViewMatrix;
// 視点の座標
uniform vec3 gs_ViewPosition;

// フォグパラメータ
uniform int   gs_FogEnable;
uniform int   gs_FogMode;
uniform float gs_FogDensity;
uniform float gs_FogStart;
uniform float gs_FogEnd;
// フォグの影響度
out float v_FogFactor;

// フォグの影響度を計算する
float FogFactor(vec4 worldPosition) {
    if (gs_FogEnable == 0) {
        return 1.0;
    }
    float fogDistance = distance(worldPosition.xyz, gs_ViewPosition.xyz);
    float fogFactor = 0.0;
    if (gs_FogMode == 0) {
        // GL_EXP
	     float fogExpornent = fogDistance * gs_FogDensity;
        fogFactor = exp(-fogExpornent);
    } else if (gs_FogMode == 1) {
        // GL_EXP2
	     float fogExpornent = fogDistance * gs_FogDensity;
        fogExpornent = fogExpornent * fogExpornent;
        fogFactor = exp(-fogExpornent);
    } else {
        // GL_LINEAR
        fogFactor = (gs_FogEnd - fogDistance) / (gs_FogEnd - gs_FogStart);
    }
    return clamp(fogFactor, 0.0, 1.0);
}

void main(void) {
    // ワールド変換系の頂点座標
    v_WorldPosition = in_Position;
    // ワールド座標系の法線ベクトル
    v_WorldNormal = in_Normal;
    // ワールド座標系の接ベクトル
    v_WorldTangent = in_Tangent;
    // ワールド座標系の従法線ベクトル
    v_WorldBinormal = in_Binormal;
    // テクスチャ座標の出力
    v_TexCoord = in_TexCoord;
    // テクスチャ座標2の出力
    v_TexCoord2 = in_TexCoord2;
    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * in_Position;
    // フォグの計算
    v_FogFactor = FogFactor(v_WorldPosition);
    // ビュー空間の法線の計算
    v_ViewNormal = normalize(mat3(gs_ModelViewMatrix) * in_Normal);
}
