#include "Engine/Core/Collision/AttackCollider.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"

AttackCollider::AttackCollider(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan, float delay) {
	tag_ = ActorTag::Collider;
    name_ = name;
	owner_ = owner;
	damage_ = damage;

	lifespan_timer_ = lifespan;
	delay_timer_ = delay;

	collider_ = BoundingSphere{ radius };
	transform_.position(center);

	// 衝突判定を無効
	enable_collider_ = false;
}

void AttackCollider::update(float delta_time) {
	// 遅延処理と削除処理
	if (delay_timer_ <= 0.0f) {
		enable_collider_ = true;
		if (lifespan_timer_ <= 0.0f) die();
		lifespan_timer_ -= delta_time;
	}
	delay_timer_ -= delta_time;
}

void AttackCollider::draw() const {

}

void AttackCollider::react(Actor& other) {
	// 自分自身と主とは衝突しない
	if (other.tag() == tag() || other.tag() == owner_->tag()) return;
	// ダメージを与える
	Pawn* target = dynamic_cast<Pawn*>(&other);
	if (target != nullptr) target->take_damage(*this, damage_);
    // 生成主にコールバック
    Pawn* owner = dynamic_cast<Pawn*>(owner_);
    if (owner != nullptr) owner->on_hit_attack(*this);
	// 衝突したら削除
	die();
}

void AttackCollider::reactivate(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan, float delay, bool disable_clear) {
	is_dead_ = false;
    name_ = name;
	owner_ = owner;
	damage_ = damage;

	lifespan_timer_ = lifespan;
	delay_timer_ = delay;

	collider_ = BoundingSphere{ radius };
	transform_.position(center);

	// 衝突判定を無効
	enable_collider_ = false;

	// マネージャーからメモリ解放を無効化するかどうか(trueでメモリ解放対象なので反転)
	is_clear_ = !disable_clear;
}
