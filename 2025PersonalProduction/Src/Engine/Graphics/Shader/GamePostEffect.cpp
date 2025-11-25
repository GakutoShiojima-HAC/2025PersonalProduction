#include "Engine//Graphics/Shader/GamePostEffect.h"
#include <GSpixel_format.h>
#include "Engine/Graphics/Shader/Shaders.h"
#include "GameConfig.h"
#include "Engine/Utils/MyRandom.h"

#include "Engine/Graphics/Shader/PostEffects/BlurEffect.h"
#include "Engine/Graphics/Shader/PostEffects/SSAOEffect.h"
#include "Engine/Graphics/Shader/PostEffects/MaskEffect.h"
#include "Engine/Graphics/Shader/PostEffects/FogEffect.h"
#include "Engine/Graphics/Shader/PostEffects/BloomEffect.h"
#include "Engine/Graphics/Shader/PostEffects/FXAAEffect.h"

// ブルームエフェクトの対象にするテクセルの輝度のしきい値
const float BLOOM_THRESHOLD{ 0.85f };
// ブルームエフェクトの強さ
const float BLOOM_INTENSITY{ 0.15f };

// SSAOのカーネル数
const int KERNEL_SIZE{ 64 };

GamePostEffect& GamePostEffect::get_instance() {
    // static変数のインスタンスは１つ
    // インスタンス化も１回のみ
    static GamePostEffect self;
    return self;
}

void GamePostEffect::load() {
    gsLoadShader(Shader_SSAO, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/SSAO.frag");
    gsLoadShader(Shader_SSAOCombine, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/SSAOCombine.frag");
    gsLoadShader(Shader_Mask, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/MaskEffect.frag");
    //gsLoadShader(Shader_Fog, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/Fog.frag");
    gsLoadShader(Shader_BloomExtract, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/BloomExtract.frag");
    gsLoadShader(Shader_GaussianBlur, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/GaussianBlur.frag");
    gsLoadShader(Shader_BloomCombine, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/BloomCombine.frag");
    gsLoadShader(Shader_FXAA, "Resource/Private/Shader/RenderTexture.vert", "Resource/Private/Shader/FXAA.frag");
}

void GamePostEffect::clear() {
    end();

    // シェーダーの削除
    gsDeleteShader(Shader_SSAO);
    gsDeleteShader(Shader_SSAOCombine);
    gsDeleteShader(Shader_Mask);
    gsDeleteShader(Shader_Fog);
    gsDeleteShader(Shader_BloomExtract);
    gsDeleteShader(Shader_GaussianBlur);
    gsDeleteShader(Shader_BloomCombine);
    gsDeleteShader(Shader_FXAA);
}

void GamePostEffect::start() {
    // 現在のビューポートのサイズを取得 [0]x座標, [1]:y座標, [2]:幅, [3]:高さ
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    width_ = viewport[2];
    height_ = viewport[3];

    // ベースのレンダーターゲットの作成
    const GSpixelFormat base_color_buffer[] = {
        GS_PIXEL_FORMAT_R16G16B16A16_FLOAT, // カラー
        GS_PIXEL_FORMAT_R16G16B16A16_FLOAT  // 法線
    };
    gsCreateRenderTargetEx(Rt_Base, width_, height_, 2, base_color_buffer, &GS_PIXEL_FORMAT_D24_UNORM_S8_UINT, GS_TEX_LINEAR);

    // アンビエントオクルージョン用のレンダーターゲットの作成
    const GSpixelFormat ssao_color_buffer[] = { GS_PIXEL_FORMAT_R16G16B16A16_FLOAT };
    gsCreateRenderTargetEx(Rt_SSAO, width_, height_, 1, ssao_color_buffer, NULL, GS_TEX_NEAREST);
    gsCreateRenderTarget(Rt_SSAOBlurH1, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_SSAOBlurV1, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);
    gsCreateRenderTarget(Rt_SSAOCombine, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);

    // 回避演出用のレンダーターゲットの作成
    gsCreateRenderTarget(Rt_Mask, width_, height_, GS_TRUE, GS_TRUE, GS_TRUE);
    gsCreateRenderTarget(Rt_MaskCombine, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);

    // フォグ用のレンダーターゲットの作成
    //gsCreateRenderTarget(Rt_Fog, width_, height_, GS_TRUE, GS_FALSE, GS_TRUE);

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

    // SSAO用データの作成
    PostEffect::SSAO::create_sample_kernel(ssao_sample_kernel_, KERNEL_SIZE);
    PostEffect::SSAO::create_noise_texture(ssao_noise_texture_);

    // パラメータをリセット
    draw_avoid_effect_ = false;
    blur_power_ = 0.0f;
}

void GamePostEffect::end() {
    // SSAO用データをクリア
    ssao_sample_kernel_.clear();
    glDeleteTextures(1, &ssao_noise_texture_);
    ssao_noise_texture_ = 0;
    // レンダーターゲットの削除
    gsDeleteRenderTarget(Rt_Base);
    gsDeleteRenderTarget(Rt_SSAO);
    gsDeleteRenderTarget(Rt_SSAOBlurH1);
    gsDeleteRenderTarget(Rt_SSAOBlurV1);
    gsDeleteRenderTarget(Rt_SSAOCombine);
    gsDeleteRenderTarget(Rt_Mask);
    gsDeleteRenderTarget(Rt_MaskCombine);
    gsDeleteRenderTarget(Rt_Fog);
    gsDeleteRenderTarget(Rt_BloomExtract);
    gsDeleteRenderTarget(Rt_GaussianBlurH1);
    gsDeleteRenderTarget(Rt_GaussianBlurV1);
    gsDeleteRenderTarget(Rt_GaussianBlurH2);
    gsDeleteRenderTarget(Rt_GaussianBlurV2);
    gsDeleteRenderTarget(Rt_GaussianBlurH3);
    gsDeleteRenderTarget(Rt_GaussianBlurV3);
    gsDeleteRenderTarget(Rt_GaussianBlurH4);
    gsDeleteRenderTarget(Rt_GaussianBlurV4);
    gsDeleteRenderTarget(Rt_BloomCombine);
    gsDeleteRenderTarget(Rt_FXAA);
}

void GamePostEffect::draw(const GSmatrix4& projection) const {
    GSuint current = Rt_Base;

    const GSvector2 screen_size = get_screen_size();

    // アンビエントオクルージョン
    if (setting_.is_draw_ssao()) {
        // AOマップの取得
        GSuint aomap = PostEffect::SSAO::get_aomap(current, projection, ssao_noise_texture_, ssao_sample_kernel_, screen_size);
        // ブラーをかける
        aomap = PostEffect::Blur::apply_blur(aomap, screen_size, Rt_SSAOBlurH1, Rt_SSAOBlurV1);
        // 合成
        current = PostEffect::SSAO::combine(current, aomap);
    }

    // マスク
    if (is_draw_mask()) {
        current = PostEffect::Mask::mask(current, Rt_Mask, mask_color_);
    }

    // フォグ
    //current = PostEffect::Fog::fog(current, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 20.0f, 1000.0f);

    // ブルーム
    if (setting_.is_draw_bloom()) {
        // 高輝度テクセルを抽出
        GSuint extract = PostEffect::Bloom::extract(current, BLOOM_THRESHOLD);
        // ブラーをかける
        GSuint blur1 = PostEffect::Blur::apply_blur(extract, { width_ / 4.0f, height_ / 4.0f }, Rt_GaussianBlurH1, Rt_GaussianBlurV1);
        GSuint blur2 = PostEffect::Blur::apply_blur(blur1, { width_ / 8.0f,  height_ / 8.0f }, Rt_GaussianBlurH2, Rt_GaussianBlurV2);
        GSuint blur3 = PostEffect::Blur::apply_blur(blur2, { width_ / 16.0f, height_ / 16.0f }, Rt_GaussianBlurH3, Rt_GaussianBlurV3);
        GSuint blur4 = PostEffect::Blur::apply_blur(blur3, { width_ / 32.0f, height_ / 32.0f }, Rt_GaussianBlurH4, Rt_GaussianBlurV4);
        // 合成
        current = PostEffect::Bloom::combine(current, BLOOM_INTENSITY, blur1, blur2, blur3, blur4);
    }

    // アンチエイリアシング
    if (setting_.is_draw_fxaa()) {
        current = PostEffect::FXAA::fxaa(current, screen_size);
    }

    // シーンをぼかす
    if (blur_power_ > 0.0f) {
        if (blur_power_ >= 0.25f) current = PostEffect::Blur::apply_blur(current, { width_ / 4.0f, height_ / 4.0f }, Rt_GaussianBlurH1, Rt_GaussianBlurV1);
        if (blur_power_ >= 0.5f) current = PostEffect::Blur::apply_blur(current, { width_ / 8.0f,  height_ / 8.0f }, Rt_GaussianBlurH2, Rt_GaussianBlurV2);
        if (blur_power_ >= 0.75f) current = PostEffect::Blur::apply_blur(current, { width_ / 16.0f, height_ / 16.0f }, Rt_GaussianBlurH3, Rt_GaussianBlurV3);
        if (blur_power_ >= 1.0f) current = PostEffect::Blur::apply_blur(current, { width_ / 32.0f, height_ / 32.0f }, Rt_GaussianBlurH4, Rt_GaussianBlurV4);
    }

    // 最終結果のテクスチャを設定
    gsBindRenderTargetTextureEx(current, 0, 0);
    // レンダーターゲットを描画
    gsDrawRenderTarget(current);
    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(current, 0, 0);
}

void GamePostEffect::draw_start() const {
    // レンダーターゲットを有効化
    gsBeginRenderTarget(Rt_Base);
    // 深度マスク用に設定を有効化
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    // 色と深度を初期化
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GamePostEffect::draw_end() const {
    // レンダーターゲットを無効化
    gsEndRenderTarget();
}

void GamePostEffect::draw_mask_start() const {
    // 深度を再利用するためにテクスチャを取得
    GStexture* texture = gsGetRenderTargetDepthTexture(Rt_Base);
    GSuint id = texture->dwTexName;

    // レンダーターゲットを有効化
    gsBeginRenderTarget(Rt_Mask);

    // 深度情報だけ差し替え
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
    // 色を初期化
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // 深度マスク用に設定を有効化
    glDepthFunc(GL_LEQUAL);
    // 深度への書き込みを無効化
    glDepthMask(GL_FALSE);
}

void GamePostEffect::draw_mask_end() const {
    // 深度への書き込みを有効化
    glDepthMask(GL_TRUE);
    // 深度テクスチャを解除
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
    // レンダーターゲットを無効化
    gsEndRenderTarget();
}

void GamePostEffect::set_mask_color(const GScolor& color) {
    mask_color_ = color;
}

bool GamePostEffect::is_draw_mask() const {
    return draw_avoid_effect_;  // マスク処理が増えたら追記
}

bool GamePostEffect::is_draw_avoid_effect() const {
    return draw_avoid_effect_;
}

bool& GamePostEffect::enable_draw_avoid_effect() {
    return draw_avoid_effect_;
}

float& GamePostEffect::blur_power() {
    return blur_power_;
}

GSvector2 GamePostEffect::get_screen_size() const {
    return GSvector2{ (float)width_, (float)height_ };
}

