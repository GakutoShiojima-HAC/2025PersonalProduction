#include "Engine/Graphics/Shader/PostEffects/FXAAEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"

GSuint PostEffect::FXAA::fxaa(GSuint source, const GSvector2& screen_size) {
    // スクリーンサイズの逆数
    GSvector2 inv_screen_size = GSvector2{ 1.0f / screen_size.x, 1.0f / screen_size.y };

    // シェーダーを有効にする
    gsBeginShader(Shader_FXAA);
    // テクスチャの設定
    gsSetShaderParamTexture("u_SceneTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);
    // スクリーンサイズの逆数を設定
    gsSetShaderParam2f("u_InvScreenSize", &inv_screen_size);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_FXAA);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_FXAA);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);

    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_FXAA;
}
