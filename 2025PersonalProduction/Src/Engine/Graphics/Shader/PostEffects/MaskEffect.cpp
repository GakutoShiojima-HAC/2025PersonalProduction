#include "Engine/Graphics/Shader/PostEffects/MaskEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"

GSuint PostEffect::Mask::mask(GSuint source, GSuint mask, const GScolor& color) {
    // シェーダーを有効にする
    gsBeginShader(Shader_Mask);

    // テクスチャの設定
    gsSetShaderParamTexture("u_SceneTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    gsSetShaderParamTexture("u_MaskTexture", 31);
    gsBindRenderTargetTextureEx(mask, 0, 31);
    // 色の設定
    GSvector3 mask_color = color.rgb;
    gsSetShaderParam3f("u_Color", &mask_color);  // 0.6f, 0.3f, 1.0f

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_MaskCombine);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_MaskCombine);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);
    gsUnbindRenderTargetTextureEx(mask, 0, 31);

    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_MaskCombine;
}
