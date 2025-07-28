// -----------------------------------------------------------------------------------------
//  File          : TimelineCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : タイムライン用カメラ
//					カメラタイムラインを使う場合は必ずWorldに追加すること
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
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
