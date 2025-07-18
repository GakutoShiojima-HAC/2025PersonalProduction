#include "Engine/Core/Camera/Camera.h"

void Camera::update(float delta_time) {

}

void Camera::enter() {

}

void Camera::die() {
	is_dead_ = true;
}

CameraTag& Camera::tag() {
	return tag_;
}

const GStransform& Camera::transform() const {
	return transform_;
}

GStransform& Camera::transform() {
	return transform_;
}

bool Camera::is_dead() const {
	return is_dead_;
}
