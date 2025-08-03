// -----------------------------------------------------------------------------------------
//  File          : Check.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : ���낢��Ȕ�����s��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_CHECK_H_
#define MYLIB_CHECK_H_

#include <string>
#include <vector>

using namespace std;

namespace MyLib {
	/// <summary>
	/// value��args�ɓ��Ă͂܂��Ă�����^��ԋp
	/// </summary>
	/// <param name="value">= ��r���̒l(�^���R)</param>
	/// <param name="...args">= ��r��̒l(������)</param>
	template <typename T, typename... Args>
	inline bool is_in(const T& value, const Args&... args) {
		const std::initializer_list<T> values = { args... };
		return std::find(std::begin(values), std::end(values), value) != std::end(values);
	}

	/// <summary>
	/// index��vector�̒��ɑ��݂��邩�ǂ���
	/// </summary>
	/// <param name="vec">= vector</param>
	/// <param name="index">= index</param>
	/// <param name="value">= ���݂��Ă����ꍇindex�ʒu�ɂ���v�f</param>
	/// <returns>���݂��Ă�����^</returns>
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
	/// index��vector�̒��ɑ��݂��邩�ǂ���
	/// </summary>
	/// <param name="vec">= vector</param>
	/// <param name="index">= index</param>
	/// <param name="value">= ���݂��Ă����ꍇindex�ʒu�ɂ���v�f</param>
	/// <returns>���݂��Ă�����^</returns>
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
