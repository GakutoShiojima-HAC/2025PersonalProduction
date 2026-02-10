#version 330
in vec4 v_WorldPosition;                // 頂点座標
in vec3 v_WorldNormal;                  // 法線ベクトル
in vec3 v_WorldTangent;                 // 接ベクトル
in vec3 v_WorldBinormal;                // 従法線ベクトル
in vec2 v_TexCoord;                     // テクスチャ座標
in float v_FogFactor;                   // フォグの影響度
in vec3 v_ViewNormal;                   // 法線マップ用ビューベクトル

layout(location = 0) out vec4 out_FragColor;
layout(location = 1) out vec4 out_FragNormal;

// マテリアルパラメータ
uniform vec4 gs_MaterialDiffuse;
uniform vec4 gs_MaterialSpecular;
uniform vec4 gs_MaterialEmission;

// カラーパラメータ
uniform vec4 gs_Color;
uniform vec4 gs_SecondaryColor;

// テクスチャ
uniform sampler2D   gs_DiffuseTexture;
uniform sampler2D   gs_SpecularTexture;
uniform sampler2D   gs_NormalTexture;
uniform sampler2D   gs_AmbientOcclusionTexture;
uniform sampler2D   gs_EmissionTexture;

// スペキュラキューブマップ
uniform samplerCube gs_SpecularCubemap0;
uniform samplerCube gs_SpecularCubemap1;
uniform float       gs_SpeculerCubemapBlendLerp;
uniform float       gs_SpecularCubemap0_MipmapCount;

// ライトパラメータ
uniform vec4        gs_LightPosition;
uniform vec4        gs_LightAmbient;
uniform vec4        gs_LightDiffuse;
uniform vec4        gs_LightSpecular;
uniform vec3        gs_LightSpotDirection;
uniform float       gs_LightSpotExponent;
uniform float       gs_LightSpotCutoff;
uniform float       gs_LightSpotCosInnerCutoff;
uniform float       gs_LightSpotCosCutoff;
uniform float       gs_LightConstantAttenuation;
uniform float       gs_LightLinearAttenuation;
uniform float       gs_LightQuadraticAttenuation;
// リムライトパラメータ
uniform vec4        gs_RimLightColor;
// ディザ半透明のアルファ値 
uniform float        gs_DitheredTransparency;

// 視点の座標
uniform vec3 gs_ViewPosition;

// フォグカラー
uniform vec4 gs_FogColor;

// 円周率
const float PI = 3.1415926;
// 影を作る境界線
const float TOON_THRESHOLD = 0.5;
// 影の境界のボケ具合
const float TOON_SMOOTH = 0.035;
// トゥーン調の強さ
const float TOON_BLEND = 0.6;
// リムライトの強さ
const float RIM_POWER = 0.3;
// 影色に乗算する青み成分
const vec3  SHADOW_TINT = vec3(0.6, 0.6, 0.8);
// ラフネスの最小値
const float MIN_PERCEPTUAL_ROUGHNESS = 0.045;
const float MIN_ROUGHNESS            = 0.002025;

// ノンリニア空間からリニアに変換
vec3 ConvertToLinear(vec3 color) {
    return pow(color, vec3(2.2));
}

// リニア空間からノンリニア空間に変換
vec3 ConvertToNoneLinear(vec3 color) {
    return pow(color, vec3(1 / 2.2));
}

// スムースネスをラフネスに変換
float SmoothnessToPerceptualRoughness(float smoothness) {
    return (1.0f - smoothness);
}

// リニア空間のラフネスに変換
float PerceptualRoughnessToRoughness(float perceptualRoughness) {
    return perceptualRoughness * perceptualRoughness;
}

// 5乗
float Pow5(float x) {
    return x * x * x * x * x;
}

// フレネル反射
vec3 F_Schlick(float cosA, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * Pow5(1.0 - cosA);
}

// マイクロファセット分布関数 Trowbridge-Reitz (GGX)
float D_GGX(float NoH, float roughness) {
    float a2 = roughness * roughness;
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

// 可視性関数
float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness) {
    float a = roughness;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5f / (GGXV + GGXL + 1e-5);
}

// 鏡面反射BRDF（Cook-Torrance approximation）
vec3 SpecularBRDF(vec3 specularColor, float NoH, float NoV, float NoL, float LoH, float roughness) {
    float D = D_GGX(NoH, roughness);
    float V = V_SmithGGXCorrelatedFast(NoV, NoL, roughness);
    vec3 F = F_Schlick(LoH, specularColor);
    return min(vec3(1.0), D * V * F);
}

// 拡散反射
float Fd_Lambert() {
    return 1.0 / PI;
}

// 拡散反射BRDF
vec3 DiffuseBRDF(vec3 diffuseColor) {
    return diffuseColor * Fd_Lambert();
}

// 環境BRFFの計算
vec3 EnvBRDFApprox(vec3 specularColor, float roughness, float NoV) {
    const vec4 c0 = vec4(-1, -0.0275, -0.572, 0.022);
    const vec4 c1 = vec4( 1,  0.0425,  1.04, -0.04 );
    vec4 r = roughness * c0 + c1;
    float a004 = min( r.x * r.x, exp( -9.28 * NoV ) ) * r.x + r.y;
    vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
    return specularColor * AB.x + AB.y;
}

// IBL鏡面反射光マップからカラーを取得する
vec3 sampleSpecularCubemap(vec3 p, float lod) {
    vec3 cubemap0 = textureLod(gs_SpecularCubemap0, p, lod).rgb;
    vec3 cubemap1 = textureLod(gs_SpecularCubemap1, p, lod).rgb;
    return mix(cubemap0, cubemap1, gs_SpeculerCubemapBlendLerp) + gs_LightAmbient.rgb;
}

// IBLの計算
vec3 ImageBasedLighting(vec3 N, vec3 R, float NoV, vec3 diffuseColor, vec3 specularColor, float perceptualRoughness) {
    vec3 Ld  = sampleSpecularCubemap(N, gs_SpecularCubemap0_MipmapCount - 0.5).rgb * diffuseColor;// * Fd_Lambert();
    vec3 Lld = sampleSpecularCubemap(R, perceptualRoughness * gs_SpecularCubemap0_MipmapCount).xyz;
    vec3 Lr  = EnvBRDFApprox(specularColor, perceptualRoughness, NoV) * Lld;
    return Ld + Lr;
}

// 法線マップの展開
vec3 UnpackNormalDXT5nm(vec4 packednormal) {
    vec3 normal;
    normal.xy = packednormal.wy * 2.0 - 1.0;
    normal.z = sqrt(1.0 - clamp(dot(normal.xy, normal.xy), 0.0, 1.0));
    return normal;
}

// ライトの減衰を計算する
float LightAttenuation(vec3 position, vec3 light) {
	if (gs_LightPosition.w == 0.0) return 1.0;
	float d = distance(position, light);
   float attenuation = 1.0 / (gs_LightConstantAttenuation
                            + gs_LightLinearAttenuation * d
                            + gs_LightQuadraticAttenuation * d * d);
	return clamp(attenuation, 0.0, 1.0);
}
// スポットライトによる減衰を計算する
float SpotLightAttenuation(vec3 L) {
    if (gs_LightSpotCutoff == 180.0) return 1.0;
    float spotDot = max(dot(-L, normalize(gs_LightSpotDirection)), 0.0);
    return (spotDot < gs_LightSpotCosCutoff) ? 0.0:  pow(spotDot, gs_LightSpotExponent);
}


void main() {
    // アルベドカラー
    vec4 albedoColor  = texture(gs_DiffuseTexture, v_TexCoord) * gs_MaterialDiffuse;
    // リニアカラーに変換する
    albedoColor.rgb = ConvertToLinear(albedoColor.rgb);
    // メタリック・スムースネス
    vec4 metallicSmoothness = texture(gs_SpecularTexture, v_TexCoord) * gs_MaterialSpecular;
    // 拡散反射カラーの取得
    vec3 diffuseColor = mix(albedoColor.rgb, vec3(0.0), metallicSmoothness.r);
    // 鏡面反射カラーの取得（金属）
    vec3 specularColor = mix(vec3(0.04), albedoColor.rgb, metallicSmoothness.r);
    // スムースネスをラフネスに変換 0.0(ツルツル）～1.0(ザラザラ）
    float perceptualRoughness = SmoothnessToPerceptualRoughness(metallicSmoothness.a);
    // 知覚的なラフネスを変換　0.0(ツルツル）～1.0(ザラザラ）
    float roughness = PerceptualRoughnessToRoughness(perceptualRoughness);
    // ラフネスの最小値を補正する（ハイライトがなくなるの防ぐ）
    roughness = max(roughness, MIN_ROUGHNESS);
    // アンビエントオクルージョン
    float ao = texture(gs_AmbientOcclusionTexture, v_TexCoord).g;
    // エミッションカラー
    vec4 emissonColor = texture(gs_EmissionTexture, v_TexCoord) * gs_MaterialEmission;
    // リニアカラーに変換する
    emissonColor.rgb = ConvertToLinear(emissonColor.rgb);

    // 各種ベクトルの計算
    mat3 TBN = mat3(normalize(v_WorldTangent), normalize(v_WorldBinormal), normalize(v_WorldNormal));
    vec3 N = TBN * normalize(UnpackNormalDXT5nm(texture(gs_NormalTexture, v_TexCoord)));
    vec3 L = (gs_LightPosition.w == 0.0) ? normalize(gs_LightPosition.xyz) : normalize(gs_LightPosition.xyz - v_WorldPosition.xyz);
    vec3 V = normalize(gs_ViewPosition.xyz  - v_WorldPosition.xyz);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-V, N);
    float NoV = abs(dot(N, V));
    float NoL = max(0.0001, dot(N, L));
    float NoH = max(0.0001, dot(N, H));
    float LoV = max(0.0001, dot(L, V));
    float LoH = max(0.0001, dot(L, H));

    // ライトの減衰値を求める
    float attenuation = LightAttenuation(v_WorldPosition.xyz, gs_LightPosition.xyz);
    // スポットライトの減衰値を求める
    attenuation *= SpotLightAttenuation(L);

    // 拡散反射の階調化
    float ramp = smoothstep(TOON_THRESHOLD - TOON_SMOOTH, TOON_THRESHOLD + TOON_SMOOTH, NoL);
    // 法線を復活
    float finalDiffuseFactor = mix(NoL, ramp, TOON_BLEND);
    // 裏面が出ないようにする
    finalDiffuseFactor = max(0.0, finalDiffuseFactor);
    // 影色のスタイライズ
    vec3 litColor = gs_LightDiffuse.rgb * attenuation;
    vec3 shadowedColor = gs_LightAmbient.rgb * SHADOW_TINT;
    // ブレンド
    vec3 toonDiffuseLight = mix(shadowedColor, litColor, finalDiffuseFactor);
    // 最終的な拡散反射
    vec3 finalDiffuse = diffuseColor * toonDiffuseLight;

    // 放射照度の計算
    vec3 irradiance = NoL * gs_LightDiffuse.rgb * PI * attenuation;
    // 双方向反射率分布関数(BRDF)の計算
    vec3 Fd = DiffuseBRDF(diffuseColor);
    vec3 Fr = SpecularBRDF(specularColor, NoH, NoV, NoL, LoH, roughness);
    vec3 BRDF = (Fd + Fr) * irradiance;
    // イメージベースドライティング
    vec3 IBL = ImageBasedLighting(N, R, NoV, diffuseColor, specularColor, perceptualRoughness);

    // 鏡面反射の階調化
    vec3 finalSpecular = SpecularBRDF(specularColor, NoH, NoV, max(0.0, NoL), LoH, roughness);
    finalSpecular *= irradiance * 1.2;

    // リムライト
    float fresnel = pow(max(0.0, 1.0 - NoV), 4.0);
    // 光が当たっている面だけ
    float rimIntensity = fresnel * step(0.1, NoL);
    // 最終的なリムライト
    vec3 finalRim = gs_RimLightColor.rgb * rimIntensity * RIM_POWER;

    // 最終的なカラーの計算
    vec3 finalColor = (finalDiffuse + finalSpecular + IBL) * ao + emissonColor.rgb + finalRim;
    //vec3 finalColor = (BRDF + IBL) * ao + emissonColor.rgb;
    // カラー調整
    finalColor.rgb = finalColor.rgb * gs_Color.rgb + gs_SecondaryColor.rgb;
    // フォグの計算
    finalColor.rgb = mix(gs_FogColor.rgb, finalColor.rgb, v_FogFactor);
    // ノンリニアカラーに変換
    finalColor.rgb = ConvertToNoneLinear(finalColor.rgb);
    // 最終結果を計算
    out_FragColor = vec4(finalColor, albedoColor.a * gs_Color.a);

    // 正規化した法線マップ用ビューベクトル
    out_FragNormal = vec4(normalize(v_ViewNormal), 1.0f);
}
