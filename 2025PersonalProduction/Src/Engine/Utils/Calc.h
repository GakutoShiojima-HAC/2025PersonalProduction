// -----------------------------------------------------------------------------------------
//  File          : Calc.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/04
//  Updated       : 2025/09/05
//  Description   : いろいろな計算結果を返す
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_CALC_H_
#define MYLIB_CALC_H_

#include <gslib.h>

namespace MyLib {
	/// <summary>
	/// 方向を分割数に分けて返却する
	/// </summary>
	/// <param name="d">= 方向</param>
	/// <param name="f">= 前方向</param>
	/// <param name="div">= 分割数</param>
	/// <returns>前が0、そこから反時計回りに分割数 方向ベクトルが無ければ負の値を返却</returns>
	int get_direction(const GSvector2& d, const GSvector2& f, const int div) {
		if (d.magnitude() < 0.01f || div <= 0) return -1;

		// 正規化
		const GSvector2 dir = d.normalized();
		const GSvector2 fwd = f.normalized();
		// 右を求めておく
		const GSvector2 local_right = GSvector2{ fwd.y, -fwd.x };

		// 方向を計算
		const float x = GSvector2::dot(dir, fwd);
		const float y = GSvector2::dot(dir, local_right);
		const float angle_rad = std::atan2(y, x);
		const float angle_deg = angle_rad * 180.0f / GS_PI;

		// 角度の基準値
		const float slice_size = 360.0f / div;
		// 0度が前方向になるように補正
		float shifted_angle = angle_deg + slice_size / 2.0f;
		// 正規化
		if (shifted_angle < 0.0f) shifted_angle += 360.0f;
		// 分割
		int index = (int)(shifted_angle / slice_size);
		// 誤差補正かけて返却
		return index % div;
	}
}

#endif