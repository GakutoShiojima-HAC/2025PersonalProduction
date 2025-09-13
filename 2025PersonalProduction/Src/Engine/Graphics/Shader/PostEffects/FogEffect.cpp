#include "Engine/Graphics/Shader/PostEffects/FogEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"
#include "GameConfig.h"

GSuint PostEffect::Fog::fog(GSuint source, const GScolor& color, float start, float end) {
    // シェーダーを有効にする
    gsBeginShader(Shader_Fog);

    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    // デプスバッファテクスチャの設定
    gsSetShaderParamTexture("u_DepthTexture", 31);
    gsBindRenderTargetDepthEx(source, 31);
    // zバッファパラメータの設定
    GSvector4 zparams = zbuffer_params(cNEAR, cFAR);
    gsSetShaderParam4f("u_ZBufferParams", &zparams);
    // フォグのカラーの設定
    gsSetShaderParam4f("u_FogColor", &color);
    // フォグの開始位置の設定
    gsSetShaderParam1f("u_FogStart", start);
    // フォグの終了位置の設定
    gsSetShaderParam1f("u_FogEnd", end);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_Fog);
    // レンダーターゲットを描画
    gsDrawRenderTargetEx(Rt_Fog);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);
    gsUnbindRenderTargetDepthEx(source, 31);

    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_Fog;
}

GSvector4 PostEffect::Fog::zbuffer_params(float near, float far) {
    float x = 1.0f - (far / near);
    float y = (far / near);
    float z = x / far;
    float w = y / far;
    return GSvector4{ x, y, z, w };
}
