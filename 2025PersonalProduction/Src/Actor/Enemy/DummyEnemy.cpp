#include "Actor/Enemy/DummyEnemy.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Utils/Check.h"
#include "GameConfig.h"
#include "Engine/Core/Collision/AttackColliderPool.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

// è’ìÀîªíËópÇÃîºåa
const float RADIUS{ 0.5f };
// ñ≥ìGéûä‘(ïb)
const float INVINCIBLE_TIME{ 0.5f };

DummyEnemy::DummyEnemy(IWorld* world, const GSvector3& position) {
	world_ = world;
	tag_ = ActorTag::Enemy;
	name_ = "DummyEnemy";

	height_ = 2.0f;
	head_offset_ = 2.0f;
	foot_offset_ = 0.05f;

	// è’ìÀîªíËãÖÇê∂ê¨
	collider_ = BoundingSphere{ RADIUS, GSvector3{ 0.0f, height_ / 2.0f, 0.0f } };

	transform_.position(position);
	collide_field();
	mesh_.transform(transform_.localToWorldMatrix());
}

void DummyEnemy::update(float delta_time) {
	update_generate_collider(delta_time);
	update_invincible(delta_time);
	update_gravity(delta_time);
	update_mesh(delta_time);

#ifdef _DEBUG
	ImGui::Begin("Dummy Enemy Window");

	ImGui::Text("Transform");

	GSvector3 pos = transform_.position();
	ImGui::DragFloat3("Position", pos, 0.025f);
	transform_.position(pos);

	GSvector3 rot = transform_.eulerAngles();
	ImGui::DragFloat3("Rotation", rot, 5.0f);
	transform_.eulerAngles(rot);

	ImGui::Separator();
	ImGui::Text("AttackCollider");

	if (ImGui::Button("Loop Generate")) is_generate_collider_ = !is_generate_collider_;
	
	ImGui::DragFloat3("Generate Offset", generate_offset_, 0.025f);

	ImGui::End();
#endif
}

void DummyEnemy::draw() const {
	mesh_.draw();

#ifdef _DEBUG
	draw_collider();
#endif
}

void DummyEnemy::take_damage(Actor& other, const int damage) {
	invincible_timer_ = INVINCIBLE_TIME;
}

void DummyEnemy::react(Actor& other) {
	if (MyLib::is_in((GSuint)other.tag(), (GSuint)ActorTag::Player, (GSuint)ActorTag::Enemy)) collide_actor(other);
}

void DummyEnemy::update_generate_collider(float delta_time) {
	if (!is_generate_collider_) return;

	// ê∂ê¨
	if (generate_timer_ <= 0.0f) {
		GSvector3 position = local_to_world(generate_offset_, GSvector3::zero(), GSvector3::one()).position();
		world_->generate_attack_collider(collider_radius_, position, this, collider_damage_);

		generate_timer_ = generate_interval_;
	}

	generate_timer_ -= delta_time / cFPS;
}
