#include "Camera/FixedCamera.h"

FixedCamera::FixedCamera(IWorld* world, const GSvector3& position, const GSvector3& at) {
	world_ = world;
	tag_ = CameraTag::Fixed;

	transform_.position(position);
	transform_.lookAt(at);
}

void FixedCamera::die() {
	// ŠO•”‚©‚çŽ€–S”»’è‚É‚Å‚«‚È‚¢‚æ‚¤‚É‚·‚é
}
