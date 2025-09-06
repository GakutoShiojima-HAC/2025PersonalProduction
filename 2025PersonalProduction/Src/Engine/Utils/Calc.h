// -----------------------------------------------------------------------------------------
//  File          : Calc.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/04
//  Updated       : 2025/09/05
//  Description   : ���낢��Ȍv�Z���ʂ�Ԃ�
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_CALC_H_
#define MYLIB_CALC_H_

#include <gslib.h>

namespace MyLib {
	/// <summary>
	/// �����𕪊����ɕ����ĕԋp����
	/// </summary>
	/// <param name="d">= ����</param>
	/// <param name="f">= �O����</param>
	/// <param name="div">= ������</param>
	/// <returns>�O��0�A�������甽���v���ɕ����� �����x�N�g����������Ε��̒l��ԋp</returns>
	int get_direction(const GSvector2& d, const GSvector2& f, const int div) {
		if (d.magnitude() < 0.01f || div <= 0) return -1;

		// ���K��
		const GSvector2 dir = d.normalized();
		const GSvector2 fwd = f.normalized();
		// �E�����߂Ă���
		const GSvector2 local_right = GSvector2{ fwd.y, -fwd.x };

		// �������v�Z
		const float x = GSvector2::dot(dir, fwd);
		const float y = GSvector2::dot(dir, local_right);
		const float angle_rad = std::atan2(y, x);
		const float angle_deg = angle_rad * 180.0f / GS_PI;

		// �p�x�̊�l
		const float slice_size = 360.0f / div;
		// 0�x���O�����ɂȂ�悤�ɕ␳
		float shifted_angle = angle_deg + slice_size / 2.0f;
		// ���K��
		if (shifted_angle < 0.0f) shifted_angle += 360.0f;
		// ����
		int index = (int)(shifted_angle / slice_size);
		// �덷�␳�����ĕԋp
		return index % div;
	}
}

#endif