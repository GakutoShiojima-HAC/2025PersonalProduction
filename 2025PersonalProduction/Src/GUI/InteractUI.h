// -----------------------------------------------------------------------------------------
//  File          : InteractUI.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/03
//  Updated       : 2025/10/03
//  Description   : インタラクトのUI描画を行う
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef INTERACT_UI_H_
#define INTERACT_UI_H_

#include <vector>

class Actor;

namespace InteractUI {
    /// <summary>
    /// インタラクトUIを描画
    /// </summary>
    /// <param name="actors"></param>
    void draw(const std::vector<Actor*>& actors, int target_index);

}

#endif
