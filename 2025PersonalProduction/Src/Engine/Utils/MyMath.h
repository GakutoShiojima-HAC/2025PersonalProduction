// -----------------------------------------------------------------------------------------
//  File          : MyMath.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/25
//  Updated       : 2025/10/25
//  Description   : 計算関数郡
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_MYMATH_H_
#define MYLIB_MYMATH_H_

#include <gslib.h>

namespace MyMath {
    /// <summary>
    /// ターゲットへの角度を取得
    /// </summary>
    /// <param name="self_position">= 自身の座標</param>
    /// <param name="self_foward">= 自身の前方向</param>
    /// <param name="target_position">= 対象の座標</param>
    /// <returns>角度</returns>
    inline float to_target_angle(const GSvector3& self_position, const GSvector3& self_foward, const GSvector3& target_position) {
        GSvector3 to_target = target_position - self_position;
        return GSvector3::angle(self_foward, to_target);
    }



}

#endif
