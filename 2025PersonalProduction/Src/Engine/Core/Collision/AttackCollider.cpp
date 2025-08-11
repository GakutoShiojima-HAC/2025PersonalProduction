#include "Engine/Core/Collision/AttackCollider.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"

AttackCollider::AttackCollider(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan, float delay) {
	tag_ = ActorTag::Collider;
	name_ = "AttackCollider";

	owner_ = owner;
	damage_ = damage;

	lifespan_timer_ = lifespan;
	delay_timer_ = delay;

	collider_ = BoundingSphere{ radius };
	transform_.position(center);

	// �Փ˔���𖳌�
	enable_collider_ = false;
}

void AttackCollider::update(float delta_time) {
	// �x�������ƍ폜����
	if (delay_timer_ <= 0.0f) {
		enable_collider_ = true;
		if (lifespan_timer_ <= 0.0f) die();
		lifespan_timer_ -= delta_time;
	}
	delay_timer_ -= delta_time;
}

void AttackCollider::draw() const {
#ifdef _DEBUG
	draw_collider();
#endif
}

void AttackCollider::react(Actor& other) {
	// �������g�Ǝ�Ƃ͏Փ˂��Ȃ�
	if (other.tag() == tag() || other.tag() == owner_->tag()) return;
	// �_���[�W��^����
	Pawn* target = dynamic_cast<Pawn*>(&other);
	if (target != nullptr) target->take_damage(other, damage_);
	// �Փ˂�����폜
	die();
}

void AttackCollider::reactivate(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan, float delay, bool disable_clear) {
	is_dead_ = false;

	owner_ = owner;
	damage_ = damage;

	lifespan_timer_ = lifespan;
	delay_timer_ = delay;

	collider_ = BoundingSphere{ radius };
	transform_.position(center);

	// �Փ˔���𖳌�
	enable_collider_ = false;

	// �}�l�[�W���[���烁��������𖳌������邩�ǂ���(true�Ń���������ΏۂȂ̂Ŕ��])
	is_clear_ = !disable_clear;
}
