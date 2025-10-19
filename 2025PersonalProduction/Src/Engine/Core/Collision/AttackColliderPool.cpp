#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Collision/AttackCollider.h"
#include "Engine/Core/World/World.h"

AttackColliderPool::AttackColliderPool(World* world) {
	world_ = world;
}

AttackColliderPool::~AttackColliderPool() {
	clear();
}

void AttackColliderPool::generate(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan, float delay) {
	AttackCollider* collider = colliders_.get();
	collider->reactivate(radius, center, owner, damage, name, lifespan, delay, true);
	active_colliders_.push_back(collider);
	world_->add_actor(collider);
}

void AttackColliderPool::remove() {
	for (auto i = active_colliders_.begin(); i != active_colliders_.end();) {
		if ((*i)->is_dead()) {
			// マネージャーからのメモリ解放を無効化しているならプール管理下に戻す
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
		// マネージャーからのメモリ解放を無効化しているならプール管理下に戻す
		if (!collider->is_clear()) colliders_.release(collider);
	}
	active_colliders_.clear();
}
