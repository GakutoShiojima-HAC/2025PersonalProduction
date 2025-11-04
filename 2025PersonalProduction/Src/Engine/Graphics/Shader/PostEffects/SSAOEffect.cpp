#include "Engine/Graphics/Shader/PostEffects/SSAOEffect.h"
#include "Engine/Graphics/Shader/Shaders.h"
#include "Engine/Utils/MyRandom.h"

GSuint PostEffect::SSAO::get_aomap(
    GSuint source,
    const GSmatrix4& projection,
    GSint noise_texture,
    const std::vector<GSvector3>& sample_kernel,
    const GSvector2& screen_size
) {
    // シェーダーを有効にする
    gsBeginShader(Shader_SSAO);

    // 法線テクスチャの設定
    gsSetShaderParamTexture("u_Normal", 30);
    gsBindRenderTargetTextureEx(source, 1, 30);
    // 深度テクスチャの設定
    gsSetShaderParamTexture("u_Depth", 31);
    gsBindRenderTargetDepthEx(source, 31);
    // ノイズテクスチャの設定
    GLint noise_location = gsGetShaderUnirom("u_Noise");
    if (noise_location != -1) {
        glUniform1i(noise_location, 32);
        glActiveTexture(GL_TEXTURE0 + 32);
        glBindTexture(GL_TEXTURE_2D, noise_texture);
    }
    // 射影行列
    gsSetShaderParamMatrix4("u_ssaoProjection", &projection);
    // 逆射影行列
    GSmatrix4 inv_projection = projection.getInverse();
    gsSetShaderParamMatrix4("u_ssaoInvProjection", &inv_projection);
    // サンプルカーネルの設定
    gsSetShaderParamArray3f("u_SampleKernel", sample_kernel.size(), sample_kernel.data());
    // スクリーンサイズの設定
    gsSetShaderParam2f("u_ScreenSize", &screen_size);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_SSAO);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_SSAO);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 1, 30);
    gsUnbindRenderTargetDepthEx(source, 31);
    glActiveTexture(GL_TEXTURE0 + 32);
    glBindTexture(GL_TEXTURE_2D, 0);

    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_SSAO;
}

GSuint PostEffect::SSAO::combine(GSuint source, GSuint aomap) {
    // シェーダーを有効にする
    gsBeginShader(Shader_SSAOCombine);

    // テクスチャの設定
    gsSetShaderParamTexture("u_BaseTexture", 0);
    gsBindRenderTargetTextureEx(source, 0, 0);
    // SSAOマップの設定
    gsSetShaderParamTexture("u_AOTexture", 1);
    gsBindRenderTargetTextureEx(aomap, 0, 1);

    // レンダーターゲットを描画先にする
    gsBeginRenderTarget(Rt_SSAOCombine);
    // レンダーターゲットの描画
    gsDrawRenderTargetEx(Rt_SSAOCombine);

    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(source, 0, 0);
    gsUnbindRenderTargetTextureEx(aomap, 0, 1);

    // レンダーターゲットの解除　
    gsEndRenderTarget();
    // シェーダーを無効にする
    gsEndShader();

    // 最終結果を返却
    return Rt_SSAOCombine;
}

void PostEffect::SSAO::create_sample_kernel(std::vector<GSvector3>& sample_kernel, int size) {
    // 初期化
    sample_kernel.clear();

    // 線形補間
    auto lerp = [](float a, float b, float f) {
        return a + f * (b - a);
        };

    for (int i = 0; i < size; ++i) {
        GSvector3 sample = MyRandom::random_vec3(GSvector3{ -1.0f, -1.0f, 0.0f }, GSvector3{ 1.0f, 1.0f, 1.0f });
        sample = sample.normalize();    // 単位ベクトル化
        sample *= MyRandom::random_float(0.0f, 1.0f);

        // スケールをかけて、中心付近のサンプル密度を高くする
        float scale = (float)i / (float)size;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;

        sample_kernel.push_back(sample);
    }
}

void PostEffect::SSAO::create_noise_texture(GSuint& texture) {
    std::vector<GSvector3> noise_data;

    for (int i = 0; i < 16; ++i) {
        GSvector3 noise{ MyRandom::random_float(-1.0f, 1.0f), MyRandom::random_float(-1.0f, 1.0f), 0.0f };
        noise_data.push_back(noise);
    }

    // テクスチャ生成
    GSuint noise_texture;
    glGenTextures(1, &noise_texture);
    glBindTexture(GL_TEXTURE_2D, noise_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise_data[0]);

    // ブラー禁止
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // タイリング
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture = noise_texture;
}
