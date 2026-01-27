#include "Engine/Graphics/Shader/PostEffects/ImpactEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"

GSuint PostEffect::Impact::apply(GSuint source, float power) {
    // シェーダーを有効にする
    gsBeginShader(Shader_Impact);

    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    // 強さの設定
    gsSetShaderParam1f("u_Power", power);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_Impact);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_Impact);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);

    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_Impact;
}
