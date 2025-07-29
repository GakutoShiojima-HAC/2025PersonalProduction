// -----------------------------------------------------------------------------------------
//  File          : SceneTag.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : シーンタグ列挙用
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_TAG_H_
#define SCENE_TAG_H_

enum class SceneTag {
	Null,
	Title,	// ゲームタイトル
	Menu,	// ゲームメニュー
	Game,	// ゲームプレイ
	TimelineEditor,

	MAX_SIZE	// 要素数
};

#endif
