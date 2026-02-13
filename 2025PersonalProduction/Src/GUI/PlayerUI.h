// -----------------------------------------------------------------------------------------
//  File          : PlayerUI.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/25
//  Updated       : 2025/11/25
//  Description   : プレイヤーのUI描画を行う
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_UI_H_
#define PLAYER_UI_H_

#include <string>

namespace PlayerUI {
    /// <summary>
    /// 体力を描画
    /// </summary>
    /// <param name="current">= 現在の体力</param>
    /// <param name="display">= アニメーション用の体力</param>
    /// <param name="max">= 体力の最大値</param>
    void draw_hp(int current, float display, int max);

    /// <summary>
    /// レベルを描画
    /// </summary>
    /// <param name="level">= 現在のレベル</param>
    void draw_level(int level);

    /// <summary>
    /// 武器と武器データを描画
    /// </summary>
    void draw_weapon_data(GSuint texture, const std::string& name, int damage);

}

#endif
