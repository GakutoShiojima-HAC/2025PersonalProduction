// -----------------------------------------------------------------------------------------
//  File          : FixedCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : 固定カメラ 位置変更はトランスフォームから行うこと
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef FIXED_CAMERA_H_
#define FIXED_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"

class FixedCamera : public Camera {
public:
	FixedCamera(IWorld* world, const GSvector3& position, const GSvector3& at);

};

#endif
