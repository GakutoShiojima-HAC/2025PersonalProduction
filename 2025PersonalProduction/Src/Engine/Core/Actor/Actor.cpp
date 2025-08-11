#include "Engine/Core/Actor/Actor.h"
#include <GSeffect.h>

void Actor::update(float delta_time) {

}

void Actor::late_update(float delta_time) {

}

void Actor::draw() const {

}

void Actor::draw_transparent() const {

}

void Actor::draw_gui() const {

}

void Actor::die() {
	is_dead_ = true;
}

void Actor::react(Actor& other) {

}

void Actor::collide(Actor& other) {
	// �ǂ���̃A�N�^�[���Փ˔��肪�L���Ȃ�
	if (enable_collider_ && other.enable_collider_) {
		// �Փ˔�����s��
		if (is_collide(other)) {
			// �Փ˂����ꍇ�͏Փ˃��A�N�V�������s��
			react(other);
			other.react(*this);
		}
	}
}

bool Actor::is_dead() const {
	return is_dead_;
}

bool Actor::is_clear() const {
	return is_clear_;
}

bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

string& Actor::name() {
	return name_;
}

ActorTag& Actor::tag() {
	return tag_;
}

const GStransform& Actor::transform() const {
	return transform_;
}

GStransform& Actor::transform() {
	return transform_;
}

GSvector3 Actor::velocity() const {
	return velocity_;
}

GSvector3& Actor::velocity() {
	return velocity_;
}

BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

GSmatrix4 Actor::local_to_world(const GSvector3& position, const GSvector3& rotate, const GSvector3& scale) const {
	// �w�肳�ꂽTranslate, Rotation, Scale�̍s����쐬����
	const GSmatrix4 local_matrix = GSmatrix4::TRS(position, GSquaternion::euler(rotate), scale);
	// ���[���h��Ԃɕϊ�����
	return local_matrix * transform_.localToWorldMatrix();
}

GSuint Actor::play_effect(GSuint effect_id, const GSvector3& position, const GSvector3& rotate, const GSvector3& scale) const {
	// �G�t�F�N�g���Đ�����
	const GSmatrix4 mat = local_to_world(position, rotate, scale);
	return gsPlayEffectEx(effect_id, &mat);
}

void Actor::draw_collider() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glTranslatef(collider_.center.x, collider_.center.y, collider_.center.z);
	glutWireSphere(collider_.radius, 16, 16);
	glPopMatrix();
}
