#include "Engine/Core/World/Light.h"
#include <gslib.h>

Light::Light(GSvector4 ambient, GSvector4 diffuse, GSvector4 specular, GSvector3 position) {
	ambient_ = ambient;
	diffuse_ - diffuse;
	specular_ = specular;
	position_ = position;
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
