// -----------------------------------------------------------------------------------------
//  File          : Canvas.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : スクリーンに何かを描画するキャンバスクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------


#ifndef CANVAS_H_
#define CANVAS_H_

#include <string>
#include <gslib.h>

using namespace std;

struct ScreenData;

// 基点
enum class Anchor {
	TopLeft,		TopCenter,		TopRight,
	CenterLeft,		Center,			CenterRight,
	BottomLeft,		BottomCenter,	BottomRight
};

class Canvas {
public:
	// インスタンス生成禁止
	Canvas() = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void init();

public:
	/// <summary>
	/// テクスチャをスクリーンに描画する
	/// </summary>
	/// <param name="id">= テクスチャID</param>
	/// <param name="position">= スクリーン描画位置</param>
	/// <param name="rect">= テクスチャの矩形</param>
	/// <param name="center">= スプライトの中心位置</param>
	/// <param name="color">= カラー</param>
	/// <param name="scale">= スケール値</param>
	/// <param name="rotation">= 回転角度</param>
	/// <param name="anchor">= 基点</param>
	static void draw_texture(
		const GSuint id,
		const GSvector2& position,
		const GSrect& rect,
		const GSvector2& center = GSvector2{ 0.0f, 0.0f },
		const GSvector2& scale = GSvector2{ 1.0f, 1.0f },
		const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
		const GSfloat rotation = 0.0f,
		const Anchor anchor = Anchor::TopLeft
	);

	/// <summary>
	/// テキストをスクリーンに描画する
	/// </summary>
	/// <param name="text">= テキスト</param>
	/// <param name="position">= スクリーン描画位置</param>
	/// <param name="font_size">= フォントサイズ</param>
	/// <param name="anchor">= 基点</param>
	static void draw_text(
		const string& text,
		const GSvector2& position,
		const GSuint font_size = 20,
		const Anchor anchor = Anchor::TopLeft
	);

	/// <summary>
	/// スプライトテキストをスクリーンに描画する
	/// </summary>
	/// <param name="text">= テキスト</param>
	/// <param name="position">= スクリーン描画位置</param>
	/// <param name="font_size">= フォントサイズ</param>
	/// <param name="font_name">= フォント名</param>
	/// <param name="color">= テキストカラー</param>
	/// <param name="anchor">= 基点</param>
	/// <param name="text_anchor">= テキストの基点</param>
	static void draw_sprite_text(
		const string& text,
		const GSvector2& position,
		const GSuint font_size = 20,
		const string& font_name = "ＭＳ ゴシック",
		const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
		const Anchor anchor = Anchor::TopLeft,
		const Anchor text_anchor = Anchor::TopLeft
	);

private:
	/// <summary>
	/// 矩形から基点の座標を取得
	/// </summary>
	/// <param name="anchor">= 基点</param>
	/// <param name="rect">= 矩形</param>
	/// <returns>座標</returns>
	static GSvector2 get_anchor_position(const Anchor anchor, const GSrect& rect);

	/// <summary>
	/// 基点のスクリーン座標を取得
	/// </summary>
	/// <param name="anchor">= 基点</param>
	/// <returns>座標</returns>
	static GSvector2 get_anchor_position(const Anchor anchor);

private:
	static ScreenData* screen_data_;

public:
	// コピー禁止
	Canvas(const Canvas& other) = delete;
	Canvas& operator = (const Canvas& other) = delete;

};

#endif
