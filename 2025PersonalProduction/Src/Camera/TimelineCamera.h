// -----------------------------------------------------------------------------------------
//  File          : TimelineCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : �^�C�����C���p�J����
//					�J�����^�C�����C�����g���ꍇ�͕K��World�ɒǉ����邱��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_CAMERA_H_
#define TIMELINE_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"

class TimelineCamera : public Camera {
public:
	TimelineCamera(IWorld* world);

public:
	void die() override;

};

#endif
