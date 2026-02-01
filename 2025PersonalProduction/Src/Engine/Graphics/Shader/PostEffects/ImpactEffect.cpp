#include "Engine/Graphics/Shader/PostEffects/ImpactEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"
#include "GameConfig.h"

GSuint PostEffect::Impact::apply(GSuint source, GSuint depth, float power, const float aspect_ratio, const GSvector2& hit) {
    // シェーダーを有効にする
    gsBeginShader(Shader_Impact);

    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    // デプスバッファテクスチャの設定
    gsSetShaderParamTexture("u_DepthTexture", 31);
    gsBindRenderTargetDepthEx(depth, 31);
    // 強さの設定
    gsSetShaderParam1f("u_Power", power);
    // アスペクト比の設定
    gsSetShaderParam1f("u_AspectRatio", aspect_ratio);
    // インパクト位置の設定
    gsSetShaderParam2f("u_ImpactPosition", &hit);
    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_Impact);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_Impact);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);
    gsUnbindRenderTargetDepthEx(depth, 31);

    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_Impact;
}
