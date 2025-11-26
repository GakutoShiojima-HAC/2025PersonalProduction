#include "DissolveEffect.h"
#include "Assets.h"
#include "Engine/Graphics/Shader/Shaders.h"

// UV座標のタイリング
const GSvector2 TILLING{ 1.0f, 1.0f };
// UV座標のオフセット
const GSvector2 OFFSET{ 0.0f, 0.0f };
// エッジの幅
const float EDGE_WIDTH{ 0.0f };
// エッジのカラー
const GScolor EDGE_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
// エッジの輝度
const float EDGE_COLOR_INTENSITY{ 1.0f };

GSuint PostEffect::Dissolve::apply(GSuint source, float threshold) {
    // シェーダーを有効にする
    gsBeginShader(Shader_Dissolve);

    // 元画像テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    // マスク用テクスチャの設定
    gsSetShaderParamTexture("u_MaskTexture", 31);
    gsBindTextureEx((GSuint)TextureID::DissolveMask, 31);
    // 背景用テクスチャの設定
    gsSetShaderParamTexture("u_BackGroundTexture", 32);
    gsBindTextureEx((GSuint)TextureID::BackGround, 32);
    // UV座標のタイリング
    gsSetShaderParam2f("u_Tilling", &TILLING);
    // UV座標のオフセット
    gsSetShaderParam2f("u_Offset", &OFFSET);
    // ディゾルブするしきい値
    gsSetShaderParam1f("u_Threshold", threshold);
    // エッジの幅
    gsSetShaderParam1f("u_EdgeWidth", EDGE_WIDTH);
    // エッジのカラー
    gsSetShaderParam4f("u_EdgeColor", &EDGE_COLOR);
    // エッジの輝度
    gsSetShaderParam1f("u_EdgeColorIntensity", EDGE_COLOR_INTENSITY);
    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_Dissolve);
    // レンダーターゲットを描画
    gsDrawRenderTargetEx(Rt_Dissolve);
    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);
    gsUnbindTextureEx((GSuint)TextureID::DissolveMask, 31);
    gsUnbindTextureEx((GSuint)TextureID::BackGround, 32);
    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    return Rt_Dissolve;
}
