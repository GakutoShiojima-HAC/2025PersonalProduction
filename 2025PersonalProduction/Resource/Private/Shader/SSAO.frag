#version 330

out vec4 out_FragColor;
// テクスチャ座標
in vec2 v_TexCoord;

// 法線テクスチャ
uniform sampler2D u_Normal;
// 深度テクスチャ
uniform sampler2D u_Depth;
// ノイズテクスチャ
uniform sampler2D u_Noise;

// サンプルカーネル
uniform vec3 u_SampleKernel[64];
// 射影行列
uniform mat4 u_ssaoProjection;
// 射影逆行列
uniform mat4 u_ssaoInvProjection;
// スクリーンサイズ
uniform vec2 u_ScreenSize;

const int KERNEL_SIZE = 64;
const float RADIUS = 0.3;   // 影の広がり
const float BIAS = 0.02;    // セルフシャドウイング防止
const float CONTRAST = 0.9; // 影の濃さ

// 深度値とUV座標から、ビュー空間のポジションを復元
vec3 ReconstructPositionFromDepth(vec2 texCoord, float depth) {
    float z = depth * 2.0 - 1.0;
    vec2 ndcXY = texCoord * 2.0 - 1.0;
    vec4 clipPos = vec4(ndcXY, z, 1.0);
    vec4 viewPos = u_ssaoInvProjection * clipPos;
    return viewPos.xyz / viewPos.w;
}

void main() {
    // ピクセルの基本情報をGバッファから取得
    float depth = texture(u_Depth, v_TexCoord).r;
    vec3 normal = normalize(texture(u_Normal, v_TexCoord).rgb);
    vec3 fragPos = ReconstructPositionFromDepth(v_TexCoord, depth);
    
    // ノイズテクスチャを使って、サンプルカーネルの回転軸を決定
    vec2 noiseScale = u_ScreenSize / 4.0;
    vec3 randomVec = normalize(texture(u_Noise, v_TexCoord * noiseScale).rgb);
    
    // 回転行列(TBN)を作成
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);  
    
    float occlusion = 0.0; // 遮蔽カウンタ
    
    // サンプルカーネルをループして遮蔽率を計算
    for(int i = 0; i < KERNEL_SIZE; ++i) {
    vec3 samplePos = TBN * u_SampleKernel[i];
    samplePos = fragPos + samplePos * RADIUS;
    
    vec4 offset = vec4(samplePos, 1.0);
    offset = u_ssaoProjection * offset;
    offset.xyz /= offset.w;
    offset.xy = offset.xy * 0.5 + 0.5;
    
    float occluderDepthSample = texture(u_Depth, offset.xy).r;
    vec3 occluderPos = ReconstructPositionFromDepth(offset.xy, occluderDepthSample);
    
    float rangeCheck = smoothstep(0.0, 1.0, 1.0 - abs(fragPos.z - occluderPos.z) / RADIUS);
    occlusion += (occluderPos.z > samplePos.z + BIAS ? 1.0 : 0.0) * rangeCheck;
    }
    
    // 最終的な遮蔽率を計算
    occlusion = 1.0 - (occlusion / float(KERNEL_SIZE));
    
    // コントラストを適用して、白黒の遮蔽マップとして出力
    float finalOcclusion = pow(occlusion, CONTRAST);
    out_FragColor = vec4(finalOcclusion, finalOcclusion, finalOcclusion, 1.0);
}
