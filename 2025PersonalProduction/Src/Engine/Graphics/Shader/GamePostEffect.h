#ifndef GAME_POST_EFFECT_H_
#define GAME_POST_EFFECT_H_

#include <vector>
#include <gslib.h>
#include "Engine/Core/Setting/Setting.h"

class GamePostEffect {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    GamePostEffect() = default;

public:
    ~GamePostEffect() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static GamePostEffect& get_instance();

public:
    /// <summary>
    /// ポストエフェクトシェーダーのロード
    /// </summary>
    void load();

    /// <summary>
    /// ポストエフェクトシェーダーのクリア
    /// </summary>
    void clear();

    /// <summary>
    /// レンダーターゲットを作成
    /// </summary>
    void create();

    /// <summary>
    /// レンダーターゲットを解放
    /// </summary>
    void release();

public:
    /// <summary>
    /// レンダーターゲットを描画
    /// </summary>
    /// <param name="source">= 最終結果を持つレンダーターゲット</param>
    void draw(GSuint source) const;

    /// <summary>
    /// ポストエフェクトを適用する
    /// </summary>
    /// <param name="projection">= 射影行列</param>
    /// <returns>適用後のレンダーターゲット</returns>
    GSuint apply(const GSmatrix4& projection) const;

    /// <summary>
    /// ディゾルブエフェクトを適用する
    /// </summary>
    /// <param name="source">= 最終結果を持つレンダーターゲット</param>
    /// <returns>適用後のレンダーターゲット</returns>
    GSuint apply_dissolve(GSuint source) const;

    /// <summary>
    /// レンダーターゲットへの描画を開始
    /// </summary>
    void begin() const;

    /// <summary>
    /// レンダーターゲットへの描画を終了
    /// </summary>
    void end() const;

    /// <summary>
    /// マスク用レンダーターゲットへの描画を開始
    /// </summary>
    void begin_mask() const;

    /// <summary>
    /// マスク用レンダーターゲットへの描画を終了
    /// </summary>
    void end_mask() const;

    /// <summary>
    /// GUI用レンダーターゲットへの描画を開始
    /// </summary>
    /// <param name="source">= ベースのレンダーターゲット</param>
    void begin_gui(GSuint source) const;

    /// <summary>
    /// GUI用レンダーターゲットへの描画を終了
    /// </summary>
    void end_gui() const;

public:
    /// <summary>
    /// マスクエフェクトの色を設定
    /// </summary>
    /// <param name="color">= RGBA</param>
    void set_mask_color(const GScolor& color);

    /// <summary>
    /// マスクエフェクトを描画するかどうか
    /// </summary>
    /// <returns>描画するなら真を返却</returns>
    bool is_draw_mask() const;

    /// <summary>
    /// 回避エフェクトを描画するかどうか
    /// </summary>
    /// <returns>描画するなら真を返却</returns>
    bool is_draw_avoid_effect() const;

    /// <summary>
    /// 回避エフェクトの描画を設定
    /// </summary>
    /// <returns>参照</returns>
    bool& enable_draw_avoid_effect();

    /// <summary>
    /// 最終結果にかけるブラーの強さを指定(0.0~1.0)
    /// </summary>
    /// <returns>参照</returns>
    float& blur_power();

    /// <summary>
    /// ディゾルブエフェクトのしきい値を指定(0.0~1.0)
    /// </summary>
    /// <returns>参照</returns>
    float& dissolve_threshold();

    /// <summary>
    /// インパクトエフェクトの強さを指定(0.0~1.0)
    /// </summary>
    /// <returns>参照</returns>
    float& impact_power();

private:
    /// <summary>
    /// スクリーンサイズの取得
    /// </summary>
    /// <returns></returns>
    GSvector2 get_screen_size() const;

private:
    // 描画設定
    Setting& setting_ = Setting::get_instance();

    // スクリーンサイズ
    int width_{ 0 };
    int height_{ 0 };

    // SSAO用サンプルカーネル
    std::vector<GSvector3> ssao_sample_kernel_;
    // SSAO用ノイズテクスチャ
    GSuint ssao_noise_texture_{ 0 };

    // マスクエフェクトの色
    GScolor mask_color_{ 1.0f, 1.0f, 1.0f, 1.0f };
    // 回避エフェクトを描画するかどうか
    bool draw_avoid_effect_{ false };

    // 画面ぼかしの強さ(0.0~1.0)
    float blur_power_{ 0.0f };

    // ディゾルブのしきい値(0.0~1.0)
    float threshold_{ 1.0f };
    // インパクトの強さ
    float impact_power_{ 0.0f };

public:
    // コピー禁止
    GamePostEffect(const GamePostEffect& other) = delete;
    GamePostEffect& operator = (const GamePostEffect& other) = delete;

};

#endif
