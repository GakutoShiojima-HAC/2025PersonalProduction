#include "Actor/Enemy/DummyEnemy.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Utils/Check.h"
#include "GameConfig.h"
#include "Assets.h"
#include "Engine/Core/Collision/AttackColliderPool.h"

#include "State/DummyEnemy/DummyEnemyState.h"
#include "State/DummyEnemy/DummyEnemyAttackState.h"
#include "State/DummyEnemy/DummyEnemyDeadState.h"
#include "State/DummyEnemy/DummyEnemyIdleState.h"
#include "State/DummyEnemy/DummyEnemyMoveState.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#include "Engine/Utils/DebugMarker.h"
#endif

// 衝突判定用の半径
const float RADIUS{ 0.5f };
// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.5f };

DummyEnemy::DummyEnemy(IWorld* world, const GSvector3& position) {
	world_ = world;
	tag_ = ActorTag::Enemy;
	name_ = "DummyEnemy";

	height_ = 2.0f;
	head_offset_ = 2.0f;
	foot_offset_ = 0.05f;

	// 衝突判定球を生成
	collider_ = BoundingSphere{ RADIUS, GSvector3{ 0.0f, height_ / 2.0f, 0.0f } };

	mesh_ = { (GSuint)MeshID::DummyEnemy, (GSuint)MeshID::DummyEnemy, (GSuint)MeshID::DummyEnemy };
	add_state();
	change_state((GSuint)DummyEnemyStateType::Idle, Motion::Idle, true);

	transform_.position(position);
	collide_field();
	mesh_.transform(transform_.localToWorldMatrix());

	// ナビメッシュ追加
	navmesh_ = { this, world_->navmesh() };

	// 攻撃アニメーションイベント
	mesh_.add_animation_event(Motion::Attack, 35.0f, [=] {generate_attack_collider(); });
}

void DummyEnemy::update(float delta_time) {
	update_invincible(delta_time);
	update_state(delta_time);
	update_gravity(delta_time);
	update_mesh(delta_time);

#ifdef _DEBUG
	auto state_string = [](DummyEnemyStateType s) {
		switch (s) {
		case DummyEnemyStateType::Idle:
			return "idle";
		case DummyEnemyStateType::Attack:
			return "attack";
		case DummyEnemyStateType::Dead:
			return "dead";
		case DummyEnemyStateType::Move:
			return "move";
		default:
			return "other";
		}
	};

	ImGui::Begin("Dummy Enemy Window");

	ImGui::Text("current state is %s.", state_string(DummyEnemyStateType(state_.get_current_state())));

	ImGui::Text("Transform");

	GSvector3 pos = transform_.position();
	ImGui::DragFloat3("Position", pos, 0.025f);
	transform_.position(pos);

	GSvector3 rot = transform_.eulerAngles();
	ImGui::DragFloat3("Rotation", rot, 5.0f);
	transform_.eulerAngles(rot);

	ImGui::Separator();
	ImGui::Text("AttackCollider");

	std::string toggle_generate = "Toggle Generate current: ";
	toggle_generate += is_generate_collider_ ? "on" : "off";
	if (ImGui::Button(toggle_generate.c_str())) is_generate_collider_ = !is_generate_collider_;
	
	ImGui::DragFloat3("Generate Offset", generate_offset_, 0.025f);

	ImGui::Separator();
	ImGui::Text("NavMesh Debugger Option");

	ImGui::DragFloat3("move to", move_to_, 0.1f);

	if (ImGui::Button("Search Path")) navmesh_.find_path(transform_.position(), move_to_);
	if (ImGui::Button("Move Start")) move_start();

	ImGui::End();
#endif
}

void DummyEnemy::draw() const {
	mesh_.draw();

#ifdef _DEBUG
	navmesh_.draw_path();
#endif
}

void DummyEnemy::take_damage(Actor& other, const int damage) {
	if (state_.get_current_state() == (GSuint)DummyEnemyStateType::Dead) return;

	invincible_timer_ = INVINCIBLE_TIME;

	hp_ = CLAMP(hp_ - damage, 0, INT_MAX);

	if (hp_ <= 0) {
		change_state((GSuint)DummyEnemyStateType::Dead, Motion::Dead, false);
		return;
	}
}

bool DummyEnemy::is_dead_state() const {
	return (state_.get_current_state() == (GSuint)DummyEnemyStateType::Dead) || is_dead_;
}

void DummyEnemy::react(Actor& other) {
	if (MyLib::is_in((GSuint)other.tag(), (GSuint)ActorTag::Player, (GSuint)ActorTag::Enemy)) collide_actor(other);
}

void DummyEnemy::add_state() {
	state_.add_state((GSuint)DummyEnemyStateType::Attack, make_shared<DummyEnemyAttackState>(*this));
	state_.add_state((GSuint)DummyEnemyStateType::Move, make_shared<DummyEnemyMoveState>(*this));
	state_.add_state((GSuint)DummyEnemyStateType::Dead, make_shared<DummyEnemyDeadState>(*this));
	state_.add_state((GSuint)DummyEnemyStateType::Idle, make_shared<DummyEnemyIdleState>(*this));
}

bool DummyEnemy::is_move_end() const {
	return navmesh_.end_move();
}

void DummyEnemy::update_move(float delta_time) {
	if (is_move_end()) return;

	const float MOVE_SPEED{ 0.085f };
	const float ROTATE_ANGLE{ 2.0f };
	navmesh_.update_move(delta_time, MOVE_SPEED, ROTATE_ANGLE);
}

bool DummyEnemy::is_generate_collider() const {
	return is_generate_collider_;
}

void DummyEnemy::generate_attack_collider() {
	GSvector3 position = local_to_world(generate_offset_, GSvector3::zero(), GSvector3::one()).position();
	world_->generate_attack_collider(collider_radius_, position, this, collider_damage_, 10.0f, 0.0f);
}

void DummyEnemy::move_start() {
	if (!navmesh_.found_path()) return;

	// 初期化
	navmesh_.reset_move();

	change_state((GSuint)DummyEnemyStateType::Move, DummyEnemyMotion::Move, true);
}

