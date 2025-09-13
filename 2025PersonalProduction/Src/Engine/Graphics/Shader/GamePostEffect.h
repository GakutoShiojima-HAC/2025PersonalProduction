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
    /// シェーダーのロード
    /// </summary>
    void load();

    /// <summary>
    /// シェーダーのクリア
    /// </summary>
    void clear();

    /// <summary>
    /// ポストエフェクトの使用を開始
    /// レンダーターゲットを作成する
    /// </summary>
    void start();

    /// <summary>
    /// ポストエフェクトの使用を終了
    /// レンダーターゲットを削除する
    /// </summary>
    void end();

    /// <summary>
    /// ポストエフェクトを適用した結果を描画する
    /// </summary>
    /// <param name="projection">= 射影行列</param>
    void draw(const GSmatrix4& projection) const;

    /// <summary>
    /// レンダーターゲットを有効にする
    /// 描画の前に実行する
    /// </summary>
    void draw_start() const;

    /// <summary>
    /// レンダーターゲットを無効にする
    /// 描画の後に実行する
    /// </summary>
    void draw_end() const;

public:
    /// <summary>
    /// マスク用レンダーターゲットを有効にする
    /// </summary>
    void draw_mask_start() const;

    /// <summary>
    /// マスク用レンダーターゲットを無効にする
    /// </summary>
    void draw_mask_end() const;

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
    int ssao_noise_texture_;

    // マスクエフェクトの色
    GScolor mask_color_{ 1.0f, 1.0f, 1.0f, 1.0f };
    // 回避エフェクトを描画するかどうか
    bool draw_avoid_effect_{ false };

public:
    // コピー禁止
    GamePostEffect(const GamePostEffect& other) = delete;
    GamePostEffect& operator = (const GamePostEffect& other) = delete;

};

#endif
