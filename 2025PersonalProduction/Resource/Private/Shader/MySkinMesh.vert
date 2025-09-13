#version 330
layout(location = 0)  in vec4 in_Position;      // 頂点座標
layout(location = 1)  in vec4 in_BoneWeight;    // ボーンのウェイト
layout(location = 2)  in vec3 in_Normal;        // 法線ベクトル
layout(location = 7)  in vec4 in_BoneIndices;   // ボーンのインデクス
layout(location = 8)  in vec2 in_TexCoord;      // テクスチャ座標
layout(location = 14) in vec3 in_Tangent;       // 接ベクトル
layout(location = 15) in vec3 in_Binormal;      // 従法線ベクトル

out vec4 v_WorldPosition;                       // 頂点座標 
out vec3 v_WorldNormal;                         // 法線ベクトル
out vec3 v_WorldTangent;                        // 接ベクトル
out vec3 v_WorldBinormal;                       // 従法線ベクトル
out vec2 v_TexCoord;                            // テクスチャ座標
out vec3 v_ViewNormal;                          // 法線マップ用ビューベクトル

// ワールド変換行列
uniform mat4 gs_ModelMatrix;
// 視野変換行列
uniform mat4 gs_ViewMatrix;
// 法線ベクトル変換行列
uniform mat4 gs_NormalMatrix;
// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;
// モデルビュー変換行列
uniform mat4 gs_ModelViewMatrix;
// 視点の座標
uniform vec3 gs_ViewPosition;
// ボーンの変換行列
uniform mat4 gs_BoneMatrices[200];

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
    // スキニング用の変換行列の計算
    mat4 matLocal = gs_BoneMatrices[int(in_BoneIndices.x)] * in_BoneWeight.x
                  + gs_BoneMatrices[int(in_BoneIndices.y)] * in_BoneWeight.y
                  + gs_BoneMatrices[int(in_BoneIndices.z)] * in_BoneWeight.z
                  + gs_BoneMatrices[int(in_BoneIndices.w)] * in_BoneWeight.w;
    // スキニング後のローカル座標の計算
    vec4 localPosition = matLocal       * in_Position;
    vec3 localNormal   = mat3(matLocal) * in_Normal;
    vec3 localTangent  = mat3(matLocal) * in_Tangent;
    vec3 localBinormal = mat3(matLocal) * in_Binormal;
    // ワールド変換
    v_WorldPosition = gs_ModelMatrix * localPosition;
    v_WorldNormal   = mat3(gs_ModelMatrix) * localNormal;
    v_WorldTangent  = mat3(gs_ModelMatrix) * localTangent;
    v_WorldBinormal = mat3(gs_ModelMatrix) * localBinormal;
    // テクスチャ座標の出力
    v_TexCoord = in_TexCoord;
    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * localPosition;
    // フォグの計算
    v_FogFactor = FogFactor(v_WorldPosition);
    // ビュー空間の法線の計算
    v_ViewNormal = normalize(mat3(gs_NormalMatrix) * in_Normal);
}
