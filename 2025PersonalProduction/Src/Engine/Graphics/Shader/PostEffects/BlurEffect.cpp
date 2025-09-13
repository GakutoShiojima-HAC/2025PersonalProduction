#include "Engine/Graphics/Shader/PostEffects/BlurEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"

GSuint PostEffect::Blur::apply_blur(GSuint source, GSvector2 size, GSuint blur_h, GSuint blur_v) {
    // ブラーテクスチャのテクセルサイズ
    GSvector2 blur_texel_size{ 1.0f / size.x, 1.0f / size.y };
    // 水平ブラーの方向
    GSvector2 blur_h_direction{ 1.0f, 0.0f };
    // 垂直ブラーの方向
    GSvector2 blur_v_direction{ 0.0f, 1.0f };

    // シェーダーを有効にする
    gsBeginShader(Shader_GaussianBlur);

    // テクセルサイズの設定
    gsSetShaderParam2f("u_TexelSize", &blur_texel_size);
    // ブラーの方向を水平方向にする
    gsSetShaderParam2f("u_Direction", &blur_h_direction);
    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 30);
    gsBindRenderTargetTextureEx(source, 0, 30);

    // 水平方向ブラー用のレンダーターゲットを描画先にする
    gsBeginRenderTarget(blur_h);
    // レンダーターゲットを描画
    gsDrawRenderTargetEx(blur_h);

    // テクスチャの解除
    gsUnbindRenderTargetTextureEx(source, 0, 30);
    // レンダーターゲットの解除
    gsEndRenderTarget();

    // ブラーの方向を垂直方向にする
    gsSetShaderParam2f("u_Direction", &blur_v_direction);
    // テクスチャの設定
    gsBindRenderTargetTextureEx(blur_h, 0, 30);

    // 垂直方向ブラー用のレンダーターゲットを描画先にする
    gsBeginRenderTarget(blur_v);
    // レンダーターゲットを描画
    gsDrawRenderTargetEx(blur_v);

    // テクスチャの解除
    gsUnbindRenderTargetTextureEx(blur_h, 0, 30);
    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return blur_v;
}
