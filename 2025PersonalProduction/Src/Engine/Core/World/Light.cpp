#include "Engine/Core/World/Light.h"
#include <gslib.h>

constexpr float DISTANCE = 200.0f;

Light::Light(const GSvector3& angle, const GSvector4& ambient, const GSvector4& diffuse, const GSvector4& specular) {
	ambient_ = ambient;
	diffuse_ - diffuse;
	specular_ = specular;

    const float x_rad = angle.x * (GS_PI / 180.0f);
    const float y_rad = angle.y * (GS_PI / 180.0f);
    const float sinx = std::sin(x_rad);
    const float cosx = std::cos(x_rad);
    const float siny = std::sin(y_rad);
    const float cosy = std::cos(y_rad);

    position_.x = -siny * cosy * DISTANCE;
    position_.y = sinx * DISTANCE;
    position_.z = cosy * cosx * DISTANCE;
}

void Light::draw() const {
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_);
	glLightfv(GL_LIGHT0, GL_POSITION, GSvector4{ position_.x, position_.y, position_.z, 0.0f });
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// シャドウマップのライトの位置を設定
	gsSetShadowMapLightPosition(&position_);
}

GSvector4& Light::ambient() {
	return ambient_;
}

GSvector4& Light::diffuse() {
	return diffuse_;
}

GSvector4& Light::specular() {
	return specular_;
}

GSvector3& Light::position() {
	return position_;
}
