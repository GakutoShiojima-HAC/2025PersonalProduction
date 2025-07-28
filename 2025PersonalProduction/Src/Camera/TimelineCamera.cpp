#include "Camera/TimelineCamera.h"

TimelineCamera::TimelineCamera(IWorld* world) {
	world_ = world;
	tag_ = CameraTag::Timeline;

	transform_.position(GSvector3{ 0.0f, 0.0f, 0.0f });
	transform_.lookAt(GSvector3{ 0.0f, 0.0f, 0.0f });
}

void TimelineCamera::die() {
	// �O�����玀�S����ɂł��Ȃ��悤�ɂ���
}
