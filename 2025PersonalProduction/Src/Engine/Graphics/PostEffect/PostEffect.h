// -----------------------------------------------------------------------------------------
//  File          : PostEffect.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/07
//  Updated       : 2025/09/07
//  Description   : ポストエフェクト
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef POST_EFFECT_H_
#define POST_EFFECT_H_

#include "Engine/Core/Setting/Setting.h"

// レンダーターゲット
enum {
	Rt_Base,				// ベースのレンダーターゲット

	Rt_AvoidMask,			// 回避演出用のマスク用
	Rt_AvoidEffect,			// 回避演出のポストエフェクト用

	Rt_BloomExtract,		// 高輝度テクセル抽出

	Rt_GaussianBlurH1,		// 水平方向ブラー(1/4縮小バッファ)
	Rt_GaussianBlurV1,		// 垂直方向ブラー(1/4縮小バッファ)
	Rt_GaussianBlurH2,		// 水平方向ブラー(1/8縮小バッファ)
	Rt_GaussianBlurV2,		// 垂直方向ブラー(1/8縮小バッファ)
	Rt_GaussianBlurH3,		// 水平方向ブラー(1/16縮小バッファ)
	Rt_GaussianBlurV3,		// 垂直方向ブラー(1/16縮小バッファ)
	Rt_GaussianBlurH4,		// 水平方向ブラー(1/32縮小バッファ)
	Rt_GaussianBlurV4,		// 垂直方向ブラー(1/32縮小バッファ)

	Rt_BloomCombine,		// ブルームエフェクト合成

	Rt_FXAA,				// アンチエイリアシング処理
};

// シェーダー
enum {
	Shader_AvoidEffect,		// 回避演出
	Shader_BloomExtract,    // 高輝度ピクセル抽出
	Shader_GaussianBlur,    // ガウシアンブラー
	Shader_BloomCombine,    // ブルームエフェクト合成
	Shader_FXAA,			// アンチエイリアシング
};

class PostEffect {
public:
	PostEffect() = default;

	~PostEffect() = default;

public:
	void init();

	void clear();
	
public:
	/// <summary>
	/// ポストエフェクト用レンダーターゲットに描画を開始する
	/// </summary>
	void start() const;

	/// <summary>
	/// ポストエフェクト用レンダーターゲットへの描画を終了する
	/// </summary>
	void end() const;

	/// <summary>
	/// 結果を描画する
	/// </summary>
	void draw() const;

	/// <summary>
	/// 回避演出用レンダーターゲットに描画を開始する
	/// </summary>
	void start_avoid_effect() const;

	/// <summary>
	/// 回避演出用レンダーターゲットへの描画を終了する
	/// </summary>
	void end_avoid_effect() const;

public:
	/// <summary>
	/// 回避演出エフェクトを有効にする
	/// </summary>
	/// <returns>参照</returns>
	bool& enable_avoid_effect();
	
	/// <summary>
	/// 回避演出を描画中かどうか
	/// </summary>
	/// <returns>描画中なら真を返却</returns>
	bool is_draw_avoid_effect() const;

	/// <summary>
	/// 回避演出の色を設定
	/// </summary>
	/// <param name="color">RGB</param>
	void set_avoid_color(const GSvector3& color);

private:
	/// <summary>
	/// 回避エフェクト
	/// </summary>
	/// <param name="source">= 元画像のレンダーターゲット</param>
	/// <returns>ポストエフェクトを適用したレンダーターゲット</returns>
	GSuint apply_avoid_effect(GSuint source, GSuint mask) const;

	/// <summary>
	/// ガウシアンブラー
	/// </summary>
	/// <param name="source">= 元画像のレンダーターゲット</param>
	/// <param name="size">= 縮小バッファのサイズ</param>
	/// <param name="blur_h">= 水平方向ブラー用のレンダーターゲット</param>
	/// <param name="blur_v">= 垂直方向ブラー用のレンダーターゲット</param>
	void gaussian_blur(GSuint source, GSvector2 size, GSuint blur_h, GSuint blur_v) const;

	/// <summary>
	/// ブルームに使う高輝度テクセルの抽出
	/// </summary>
	GSuint bloom_extract(GSuint source) const;

	/// <summary>
	/// ブルームエフェクトを合成
	/// </summary>
	GSuint bloom_combine(GSuint source) const;

	/// <summary>
	/// アンチエイリアシング
	/// </summary>
	GSuint apply_fxaa(GSuint source) const;

private:
	GLint width_{ 0 };
	GLint height_{ 0 };

	Setting& setting_ = Setting::get_instance();

	// 回避演出中かどうか
	bool is_avoid_effect_{ false };
	// 回避演出に使う色
	GSvector3 avoid_color_{ 0.2f, 0.4f, 1.0f };

public:
	// コピー禁止
	PostEffect(const PostEffect& other) = delete;
	PostEffect& operator = (const PostEffect& other) = delete;

};

#endif
