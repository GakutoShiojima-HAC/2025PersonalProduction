// -----------------------------------------------------------------------------------------
//  File          : Check.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : いろいろな判定を行う
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_CHECK_H_
#define MYLIB_CHECK_H_

#include <string>
#include <vector>

using namespace std;

namespace MyLib {
	/// <summary>
	/// valueがargsに当てはまっていたら真を返却
	/// </summary>
	/// <param name="value">= 比較元の値(型自由)</param>
	/// <param name="...args">= 比較先の値(複数可)</param>
	template <typename T, typename... Args>
	inline bool is_in(const T& value, const Args&... args) {
		const std::initializer_list<T> values = { args... };
		return std::find(std::begin(values), std::end(values), value) != std::end(values);
	}

	/// <summary>
	/// indexがvectorの中に存在するかどうか
	/// </summary>
	/// <param name="vec">= vector</param>
	/// <param name="index">= index</param>
	/// <param name="value">= 存在していた場合index位置にある要素</param>
	/// <returns>存在していたら真</returns>
	template <typename T>
	inline bool is_valid_index(const vector<T*>& vec, unsigned int index, T** value = nullptr) {
		if (vec.empty()) return false;
		if (index >= 0 && (size_t)index < vec.size()) {
			*value = vec[index];
			return true;
		}
		*value = nullptr;
		return false;
	}

	/// <summary>
	/// indexがvectorの中に存在するかどうか
	/// </summary>
	/// <param name="vec">= vector</param>
	/// <param name="index">= index</param>
	/// <param name="value">= 存在していた場合index位置にある要素</param>
	/// <returns>存在していたら真</returns>
	template <typename T>
	inline bool is_valid_index(const vector<T>& vec, unsigned int index, T* value) {
		if (vec.empty()) return false;
		if (index >= 0 && (size_t)index < vec.size()) {
			*value = vec[index];
			return true;
		}
		return false;
	}
}

#endif
