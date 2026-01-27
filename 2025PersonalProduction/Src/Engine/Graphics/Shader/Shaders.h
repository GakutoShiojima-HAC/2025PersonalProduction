#ifndef SHADERS_H_
#define SHADERS_H_

// レンダーターゲット
enum {
    Rt_Base,				// ベースのレンダーターゲット

    Rt_SSAO,				// アンビエントオクルージョン
    Rt_SSAOBlurH1,		    // 水平方向ブラー
    Rt_SSAOBlurV1,		    // 垂直方向ブラー
    Rt_SSAOCombine,		    // アンビエントオクルージョン合成

    Rt_Mask,			    // マスク用
    Rt_MaskCombine,		    // マスク合成結果

    Rt_Fog,					//フォグ用

    Rt_GaussianBlurH1,		// 水平方向ブラー(1/4縮小バッファ)
    Rt_GaussianBlurV1,		// 垂直方向ブラー(1/4縮小バッファ)
    Rt_GaussianBlurH2,		// 水平方向ブラー(1/8縮小バッファ)
    Rt_GaussianBlurV2,		// 垂直方向ブラー(1/8縮小バッファ)
    Rt_GaussianBlurH3,		// 水平方向ブラー(1/16縮小バッファ)
    Rt_GaussianBlurV3,		// 垂直方向ブラー(1/16縮小バッファ)
    Rt_GaussianBlurH4,		// 水平方向ブラー(1/32縮小バッファ)
    Rt_GaussianBlurV4,		// 垂直方向ブラー(1/32縮小バッファ)

    Rt_BloomExtract,		// 高輝度テクセル抽出
    Rt_BloomCombine,		// ブルームエフェクト合成

    Rt_FXAA,				// アンチエイリアシング処理

    Rt_Dissolve,            // ディゾルブ

    Rt_Impact,              // インパクトエフェクト
};

// シェーダー
enum {
    // デフォルトシェーダーをポストエフェクト用に改造したシェーダー
    Shader_Instanced,
    Shader_Mesh,
    Shader_SkinMesh,
    Shader_Octree,

    Shader_SSAO,			// アンビエントオクルージョン
    Shader_SSAOCombine,     // アンビエントオクルージョン合成
    Shader_Mask,		    // マスク
    Shader_Fog,				// フォグ
    Shader_BloomExtract,    // 高輝度ピクセル抽出
    Shader_GaussianBlur,    // ガウシアンブラー
    Shader_BloomCombine,    // ブルームエフェクト合成
    Shader_FXAA,			// アンチエイリアシング

    Shader_Dissolve,        // ディゾルブ
    Shader_Impact,          // インパクトエフェクト
};

#endif
