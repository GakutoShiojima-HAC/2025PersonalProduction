// -----------------------------------------------------------------------------------------
//  File          : FixedCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : �Œ�J���� �ʒu�ύX�̓g�����X�t�H�[������s������
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef FIXED_CAMERA_H_
#define FIXED_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"

class FixedCamera : public Camera {
public:
	FixedCamera(IWorld* world, const GSvector3& position, const GSvector3& at);

};

#endif
