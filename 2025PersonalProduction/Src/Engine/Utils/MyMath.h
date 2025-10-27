// -----------------------------------------------------------------------------------------
//  File          : MyMath.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/25
//  Updated       : 2025/10/25
//  Description   : �v�Z�֐��S
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_MYMATH_H_
#define MYLIB_MYMATH_H_

#include <gslib.h>

namespace MyMath {
    /// <summary>
    /// �^�[�Q�b�g�ւ̊p�x���擾
    /// </summary>
    /// <param name="self_position">= ���g�̍��W</param>
    /// <param name="self_foward">= ���g�̑O����</param>
    /// <param name="target_position">= �Ώۂ̍��W</param>
    /// <returns>�p�x</returns>
    inline float to_target_angle(const GSvector3& self_position, const GSvector3& self_foward, const GSvector3& target_position) {
        GSvector3 to_target = target_position - self_position;
        return GSvector3::angle(self_foward, to_target);
    }



}

#endif
