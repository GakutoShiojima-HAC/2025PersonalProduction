#include "Engine/Graphics/Shader/PostEffects/BloomEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"

GSuint PostEffect::Bloom::extract(GSuint source, float threshold) {
    // シェーダーを有効にする
    gsBeginShader(Shader_BloomExtract);

    // ブルームの対象にするピクセルの輝度を設定
    gsSetShaderParam1f("u_BloomThreshold", threshold);
    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_BloomExtract);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_BloomExtract);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);

    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_BloomExtract;
}

GSuint PostEffect::Bloom::combine(GSuint source, float intensity, GSuint blur1, GSuint blur2, GSuint blur3, GSuint blur4) {
    // シェーダーを有効にする
    gsBeginShader(Shader_BloomCombine);

    // テクスチャの設定
    gsSetShaderParamTexture("u_BaseTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    // ブラーテクスチャを設定
    gsSetShaderParamTexture("u_BlurTexture1", 31);
    gsBindRenderTargetTextureEx(blur1, 0, 31);
    gsSetShaderParamTexture("u_BlurTexture2", 32);
    gsBindRenderTargetTextureEx(blur2, 0, 32);
    gsSetShaderParamTexture("u_BlurTexture3", 33);
    gsBindRenderTargetTextureEx(blur3, 0, 33);
    gsSetShaderParamTexture("u_BlurTexture4", 34);
    gsBindRenderTargetTextureEx(blur4, 0, 34);
    gsSetShaderParam1f("u_BloomIntensity", intensity);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_BloomCombine);
    // ブルーム合成用レンダーターゲットを描画
    gsDrawRenderTargetEx(Rt_BloomCombine);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);
    // ブラーテクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(blur1, 0, 31);
    gsUnbindRenderTargetTextureEx(blur2, 0, 32);
    gsUnbindRenderTargetTextureEx(blur3, 0, 33);
    gsUnbindRenderTargetTextureEx(blur4, 0, 34);

    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_BloomCombine;
}
