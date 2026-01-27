// -----------------------------------------------------------------------------------------
//  File          : GameConfig.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : ゲーム構成
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef GAME_CONFIG_H_
#define GAME_CONFIG_H_

#include <string>

// FPS
const float cFPS{ 60.0f };

// 視野角
const float cFOV{ 45.0f };
// 近クリップ
const float cNEAR{ 0.3f };
// 遠クリップ
const float cFAR{ 1000.0f };

// 共通フォント
const std::string cFONT{ "DelaSuko Gothic One" };
// 共通フォント(テキスト用)
const std::string cFONT_TEXT{ "HackGen35 Console" };

#endif
