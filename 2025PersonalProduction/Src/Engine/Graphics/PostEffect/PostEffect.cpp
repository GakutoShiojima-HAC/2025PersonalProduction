#include "Engine/Graphics/PostEffect/PostEffect.h"
#include <gslib.h>

// ブルームエフェクトの対象にするテクセルの輝度のしきい値
const float BLOOM_THRESHOLD{ 0.9f };
// ブルームエフェクトの強さ
const float BLOOM_INTENSITY{ 0.2f };

void PostEffect::init() {
    // 現在のビューポートのサイズを取得 [0]x座標, [1]:y座標, [2]:幅, [3]:高さ
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    width_ = viewport[2];
    height_ = viewport[3];

    // ベースのレンダーターゲットの作成
    gsCreateRenderTarget(Rt_Base, width_, height_, GS_TRUE, GS_TRUE, GS_TRUE);
    // 回避演出用のレンダーターゲットの作成
    gsCreateRenderTarget(Rt_AvoidMask, width_, height_, GS_TRUE, GS_TRUE, GS_TRUE);
    gsCreateRenderTarget(Rt_AvoidEffect, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);
    // 高輝度抽出シェーダー用のレンダーターゲットの作成(1/4サイズ)
    gsCreateRenderTarget(Rt_BloomExtract, width_ / 4, height_ / 4, GS_TRUE, GS_FALSE, GS_TRUE);
    // ガウシアンブラー用の縮小バッファを作成
    gsCreateRenderTarget(Rt_GaussianBlurH1, width_ / 4, height_ / 4, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV1, width_ / 4, height_ / 4, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurH2, width_ / 8, height_ / 8, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV2, width_ / 8, height_ / 8, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurH3, width_ / 16, height_ / 16, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV3, width_ / 16, height_ / 16, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurH4, width_ / 32, height_ / 32, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_GaussianBlurV4, width_ / 32, height_ / 32, GS_TRUE, GS_FALSE, GS_TRUE);
    // ブルーム合成シェーダー用のレンダーターゲットの作成
    gsCreateRenderTarget(Rt_BloomCombine, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);
    // アンチエイリアシング用のレンダーターゲットの作成
    gsCreateRenderTarget(Rt_FXAA, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);

    // シェーダーの読み込み
    gsLoadShader(Shader_AvoidEffect, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/AvoidEffect.frag");
    gsLoadShader(Shader_BloomExtract, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/BloomExtract.frag");
    gsLoadShader(Shader_GaussianBlur, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/GaussianBlur.frag");
    gsLoadShader(Shader_BloomCombine, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/BloomCombine.frag");
    gsLoadShader(Shader_FXAA, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/FXAA.frag");

    // パラメータリセット
    is_avoid_effect_ = false;
}

void PostEffect::start() const {
    gsBeginRenderTarget(Rt_Base);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostEffect::end() const {
    gsEndRenderTarget();
}

void PostEffect::draw() const {
    GSuint current = Rt_Base;

    // 回避演出をかける
    if (is_avoid_effect_) current = apply_avoid_effect(current, Rt_AvoidMask);

    // ポストエフェクトをかける
    if (setting_.is_draw_posteffect()) {
        // 輝度の高い部分を取り出す
        bloom_extract(current);

        // ガウシアンブラーでぼかす
        gaussian_blur(Rt_BloomExtract, { width_ / 4.0f,  height_ / 4.0f }, Rt_GaussianBlurH1, Rt_GaussianBlurV1);
        gaussian_blur(Rt_GaussianBlurV1, { width_ / 8.0f,  height_ / 8.0f }, Rt_GaussianBlurH2, Rt_GaussianBlurV2);
        gaussian_blur(Rt_GaussianBlurV2, { width_ / 16.0f, height_ / 16.0f }, Rt_GaussianBlurH3, Rt_GaussianBlurV3);
        gaussian_blur(Rt_GaussianBlurV3, { width_ / 32.0f, height_ / 32.0f }, Rt_GaussianBlurH4, Rt_GaussianBlurV4);

        // ブルームエフェクトを合成
        current = bloom_combine(current);

        // アンチエイリアシング
        if (setting_.is_draw_fxaa()) current = apply_fxaa(current);
    }
    // 合成後の結果を表示
    gsBindRenderTargetTextureEx(current, 0, 0);
    // レンダーターゲットを描画
    gsDrawRenderTarget(current);
}

void PostEffect::start_avoid_effect() const {
    // マスク処理のために深度を再利用する
    GStexture* texture = gsGetRenderTargetDepthTexture(Rt_Base);
    GSuint id = texture->dwTexName;
    
    gsBeginRenderTarget(Rt_AvoidMask);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT, // 差し替え
        GL_TEXTURE_2D,
        id,
        0
    );

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);  // 深度への書き込みを禁止
}

void PostEffect::end_avoid_effect() const {
    glDepthMask(GL_TRUE);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        0,                   // 解除
        0
    );

    gsEndRenderTarget();
}

bool& PostEffect::enable_avoid_effect() {
    return is_avoid_effect_;
}

bool PostEffect::is_draw_avoid_effect() const {
    return is_avoid_effect_;
}

void PostEffect::set_avoid_color(const GSvector3& color) {
    avoid_color_ = color;
}

GSuint PostEffect::apply_avoid_effect(GSuint source, GSuint mask) const {
    // 回避演出用シェーダーを有効にする
    gsBeginShader(Shader_AvoidEffect);
    // テクスチャの設定
    gsSetShaderParamTexture("u_SceneTexture", 0);
    gsSetShaderParamTexture("u_MaskTexture", 1);
    gsSetShaderParam3f("u_Color", &avoid_color_);  // 0.6f, 0.3f, 1.0f
    // 回避演出用レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_AvoidEffect);
    // 回避演出用のテクスチャをバインド
    gsBindRenderTargetTextureEx(source, 0, 0);
    gsBindRenderTargetTextureEx(mask, 0, 1);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_AvoidEffect);
    // 回避演出用のテクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    gsUnbindRenderTargetTextureEx(mask, 0, 1);
    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();
    return Rt_AvoidEffect;
}

void PostEffect::gaussian_blur(GSuint source, GSvector2 size, GSuint blur_h, GSuint blur_v) const {
    // ブラーテクスチャのテクセルサイズ
    GSvector2 blur_texel_size{ 1.0f / size.x, 1.0f / size.y };
    // 水平ブラーの方向
    GSvector2 blur_h_direction{ 1.0f, 0.0f };
    // 垂直ブラーの方向
    GSvector2 blur_v_direction{ 0.0f, 1.0f };
    // ガウシアンブラー用シェーダーを有効にする
    gsBeginShader(Shader_GaussianBlur);
    // テクセルサイズの設定
    gsSetShaderParam2f("u_TexelSize", &blur_texel_size);
    // ブラーの方向を水平方向にする
    gsSetShaderParam2f("u_Direction", &blur_h_direction);
    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 0);
    // 水平方向ブラー用のレンダーターゲットを描画先にする
    gsBeginRenderTarget(blur_h);
    // 元画像のテクスチャをバインド
    gsBindRenderTargetTextureEx(source, 0, 0);
    // レンダーターゲットを描画
    gsDrawRenderTargetEx(blur_h);
    // テクスチャの解除
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // レンダーターゲットの解除
    gsEndRenderTarget();

    // ブラーの方向を垂直方向にする
    gsSetShaderParam2f("u_Direction", &blur_v_direction);
    // 垂直方向ブラー用のレンダーターゲットを描画先にする
    gsBeginRenderTarget(blur_v);
    // 水平方向ブラーのテクスチャをバインド
    gsBindRenderTargetTextureEx(blur_h, 0, 0);
    // レンダーターゲットを描画
    gsDrawRenderTargetEx(blur_v);
    // テクスチャの解除
    gsUnbindRenderTargetTextureEx(blur_h, 0, 0);
    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();
}

GSuint PostEffect::bloom_extract(GSuint source) const {
    // 高輝度ピクセル抽出シェーダーを有効にする
    gsBeginShader(Shader_BloomExtract);
    // ブルームの対象にするピクセルの輝度を設定
    gsSetShaderParam1f("u_BloomThreshold", BLOOM_THRESHOLD);
    // テクスチャの設定
    gsSetShaderParamTexture("u_RenderTexture", 0);
    // 輝度抽出用のレンダーターゲットをバインドする
    gsBeginRenderTarget(Rt_BloomExtract);
    // 元シーン画像用のテクスチャをバインド
    gsBindRenderTargetTextureEx(source, 0, 0);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_BloomExtract);
    // 元シーン画像用のテクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // レンダーターゲットの解除
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();
    return Rt_BloomExtract;
}

GSuint PostEffect::bloom_combine(GSuint source) const {
    // ブルーム合成用シェーダーを有効にする
    gsBeginShader(Shader_BloomCombine);
    gsSetShaderParamTexture("u_BaseTexture", 0);
    gsSetShaderParamTexture("u_BlurTexture1", 1);
    gsSetShaderParamTexture("u_BlurTexture2", 2);
    gsSetShaderParamTexture("u_BlurTexture3", 3);
    gsSetShaderParamTexture("u_BlurTexture4", 4);
    gsSetShaderParam1f("u_BloomIntensity", BLOOM_INTENSITY);
    // ブルーム合成用レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_BloomCombine);
    // 元シーンのテクスチャをバインド
    gsBindRenderTargetTextureEx(source, 0, 0);
    // ブラーテクスチャをバインド
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV1, 0, 1);
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV2, 0, 2);
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV3, 0, 3);
    gsBindRenderTargetTextureEx(Rt_GaussianBlurV4, 0, 4);
    // ブルーム合成用レンダーターゲットを描画
    gsDrawRenderTargetEx(Rt_BloomCombine);
    // 元シーンのテクスチャをバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // ブラーテクスチャをバインド解除
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV1, 0, 1);
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV2, 0, 2);
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV3, 0, 3);
    gsUnbindRenderTargetTextureEx(Rt_GaussianBlurV4, 0, 4);
    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();
    return Rt_BloomCombine;
}

GSuint PostEffect::apply_fxaa(GSuint source) const {
    // スクリーンサイズの逆数
    GSvector2 inv_screen_size{ 1.0f / (float)width_, 1.0f / (float)height_ };
    // アンチエイリアシング用シェーダーを有効にする
    gsBeginShader(Shader_FXAA);
    // テクスチャの設定
    gsSetShaderParamTexture("u_SceneTexture", 0);
    // スクリーンサイズの逆数を設定
    gsSetShaderParam2f("u_InvScreenSize", &inv_screen_size);
    // アンチエイリアシング用レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_FXAA);
    // テクスチャをバインド
    gsBindRenderTargetTextureEx(source, 0, 0);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_FXAA);
    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();
    return Rt_FXAA;
}
