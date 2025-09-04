#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Collision/AttackCollider.h"
#include "Engine/Core/World/World.h"

AttackColliderPool::AttackColliderPool(World* world) {
	world_ = world;
}

AttackColliderPool::~AttackColliderPool() {
	clear();
}

void AttackColliderPool::generate(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan, float delay) {
	AttackCollider* collider = colliders_.get();
	collider->reactivate(radius, center, owner, damage, lifespan, delay, true);
	active_colliders_.push_back(collider);
	world_->add_actor(collider);
}

void AttackColliderPool::remove() {
	for (auto i = active_colliders_.begin(); i != active_colliders_.end();) {
		if ((*i)->is_dead()) {
			// �}�l�[�W���[����̃���������𖳌������Ă���Ȃ�v�[���Ǘ����ɖ߂�
			if (!(*i)->is_clear()) colliders_.release(*i);
			i = active_colliders_.erase(i);
		}
		else {
			++i;
		}
	}
}

void AttackColliderPool::clear() {
	for (auto& collider : active_colliders_) {
		// �}�l�[�W���[����̃���������𖳌������Ă���Ȃ�v�[���Ǘ����ɖ߂�
		if (!collider->is_clear()) colliders_.release(collider);
	}
	active_colliders_.clear();
}
