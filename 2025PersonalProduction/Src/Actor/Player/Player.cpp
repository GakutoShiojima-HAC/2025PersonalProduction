#include "Actor/Player/Player.h"
#include "Assets.h"
#include "Engine/Core/World/IWorld.h"
#include "State/Player/PlayerState.h"
#include "Engine/Core/Camera/Camera.h"
#include "Camera/PlayerCamera.h"
#include "Engine/Utils/Check.h"

#include "State/Player/PlayerAttackState.h"
#include "State/Player/PlayerAvoidState.h"
#include "State/Player/PlayerDeadState.h"
#include "State/Player/PlayerFallState.h"
#include "State/Player/PlayerHurtState.h"
#include "State/Player/PlayerIdleState.h"
#include "State/Player/PlayerInteractState.h"
#include "State/Player/PlayerJumpState.h"
#include "State/Player/PlayerLandState.h"
#include "State/Player/PlayerMoveState.h"
#include "State/Player/PlayerSkillState.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

// �Փ˔���p�̔��a
const float RADIUS{ 0.5f };
// �ړ����̃J���������ւ̉�]�p�x
const float TURN_SPEED{ 11.5f };
// �ʏ�ړ����x
const float MOVE_SPEED{ 0.1f };
// �����ړ����x
const float SPRINT_SPEED{ 0.15f };
// �����ړ����x�{��
const float DECELERATION_SPEED{ 0.75f };
// ���G����(�b)
const float INVINCIBLE_TIME{ 0.5f };

Player::Player(IWorld* world, const GSvector3& position, const GSvector3& lookat, PlayerCamera* camera) {
	world_ = world;
	tag_ = ActorTag::Player;
	name_ = "Player";

	camera_ = camera;
	camera_->set_owner(this);

	height_ = 2.0f;
	head_offset_ = 2.0f;
	foot_offset_ = 0.05f;

	mesh_ = { (GSuint)MeshID::Player, (GSuint)MeshID::Player, (GSuint)MeshID::Player };
	add_state();
	change_state((GSuint)PlayerStateType::Move, Motion::Idle, true);
	
	// �Փ˔��苅�𐶐�
	collider_ = BoundingSphere{ RADIUS, GSvector3{ 0.0f, 1.0f, 0.0f } };

	transform_.position(position);
	transform_.lookAt(lookat);
	collide_field();
	mesh_.transform(transform_.localToWorldMatrix());
}

void Player::update(float delta_time) {
	update_invincible(delta_time);
	update_state(delta_time);
	update_gravity(delta_time);
	update_mesh(delta_time);

#ifdef _DEBUG
	auto state_string = [](PlayerStateType s) {
		switch (s) {
		case PlayerStateType::Idle:
			return "idle";
		case PlayerStateType::Fall:
			return "fall";
		case PlayerStateType::Land:
			return "land";
		case PlayerStateType::Move:
			return "move";
		case PlayerStateType::Interact:
			return "interact";
		case PlayerStateType::Attack:
			return "attack";
		case PlayerStateType::Skill:
			return "skill";
		case PlayerStateType::Jump:
			return "jump";
		case PlayerStateType::Avoid:
			return "avoid";
		case PlayerStateType::Hurt:
			return "hurt";
		case PlayerStateType::Dead:
			return "dead";
		default:
			return "other";
		}
	};
	ImGui::Begin("Player Window");

	ImGui::Text("current position is X:%.3f Y:%.3f Z:%.3f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("current state is %s.", state_string(PlayerStateType(state_.get_current_state())));

	ImGui::End();
#endif
}

void Player::late_update(float delta_time) {

	// TODO IK
}

void Player::draw() const {
	mesh_.draw();

	// state_.draw();

#ifdef _DEBUG
	draw_collider();
#endif
}

void Player::draw_gui() const {
	// TODO GUI

	// state_.draw_gui();
}

void Player::take_damage(Actor& other, const int damage) {
	if (MyLib::is_in(
		state_.get_current_state(), 
		(GSuint)PlayerStateType::Idle, 
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Skill	// �X�L���������G
	)) return;
	if (invincible_timer() > 0.0f) return;

	hp_ = CLAMP(hp_ - damage, 0, INT_MAX);

	if (hp_ <= 0) change_state((GSuint)PlayerStateType::Dead, Motion::Dead, false);
	else change_state((GSuint)PlayerStateType::Hurt, Motion::Hurt, false);
	
	invincible_timer_ = INVINCIBLE_TIME;
}

bool Player::is_dead_state() const {
	return MyLib::is_in(state_.get_current_state(), (GSuint)PlayerStateType::Dead);
}

void Player::react(Actor& other) {
	if (other.tag() == ActorTag::Enemy) collide_actor(other);
}

void Player::add_state() {
	state_.add_state((GSuint)PlayerStateType::Attack, make_shared<PlayerAttackState>(*this));
	state_.add_state((GSuint)PlayerStateType::Avoid, make_shared<PlayerAvoidState>(*this));
	state_.add_state((GSuint)PlayerStateType::Dead, make_shared<PlayerDeadState>(*this));
	state_.add_state((GSuint)PlayerStateType::Fall, make_shared<PlayerFallState>(*this));
	state_.add_state((GSuint)PlayerStateType::Hurt, make_shared<PlayerHurtState>(*this));
	state_.add_state((GSuint)PlayerStateType::Idle, make_shared<PlayerIdleState>(*this));
	state_.add_state((GSuint)PlayerStateType::Interact, make_shared<PlayerInteractState>(*this));
	state_.add_state((GSuint)PlayerStateType::Jump, make_shared<PlayerJumpState>(*this));
	state_.add_state((GSuint)PlayerStateType::Land, make_shared<PlayerLandState>(*this));
	state_.add_state((GSuint)PlayerStateType::Move, make_shared<PlayerMoveState>(*this));
	state_.add_state((GSuint)PlayerStateType::Skill, make_shared<PlayerSkillState>(*this));
}

void Player::update_move(float delta_time) {
	Camera* camera = world_->get_camera();
	// �J������̑O�������擾
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	// �J������̉E�������擾
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// ���͂���ړ��x�N�g�����Z�o
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	GSvector2 input = input_.left_axis();
	velocity += right * input.x;
	velocity += forward * input.y;

	// �ړ����Ă�����ړ��ʂ��A�ړ����Ă��Ȃ������猸������悤�Ɉړ��ʂ��v�Z
	if (velocity.magnitude() > 0.01f) {
		move_speed_ = input_.action(InputAction::GAME_Sprint) ? SPRINT_SPEED : MOVE_SPEED;
		velocity = velocity.normalized() * move_speed_ * delta_time;
	}
	else {
		velocity.x = velocity_.x;
		velocity.z = velocity_.z;
		// ����
		if (velocity.magnitude() > 0.01f) velocity *= DECELERATION_SPEED * delta_time;	// delta_time��v�Z�ł̑����̌덷�͋C�ɂ��Ȃ����̂Ƃ���
		// ��~
		else velocity = GSvector3{ 0.0f, 0.0f, 0.0f };
	}
	// �ړ��ʂ��X�V
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;

	// ��ђʈړ�
	non_penetrating_move(velocity, camera_->is_lockon() ? nullptr : &velocity, TURN_SPEED * delta_time);

	// TODO ���[�V������ݒ�
	// �ړ���ԂłȂ���ΏI��
	if (!(state_.get_current_state() == (GSuint)PlayerStateType::Move)) return;
	// ���͂�����Έړ����[�V������K�p����
	if (input.magnitude() > 0.0f) {
		// ���b�N�I�����Ȃ�8�������[�V������K�p����
		if (camera_->is_lockon()) {

		}
		// ���b�N�I�����łȂ����1�������[�V������K�p����
		else {

		}
	}
	// ���͂��Ȃ���Αҋ@���[�V������K�p����
	else {

	}
}

void Player::update_move_air(float delta_time) {
	Camera* camera = world_->get_camera();
	// �J������̑O�������擾
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	// �J������̉E�������擾
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// ���͂����]�x�N�g�����Z�o
	GSvector3 rotate_velocity{ 0.0f, 0.0f, 0.0f };
	GSvector2 input = input_.left_axis();
	rotate_velocity += right * input.x;
	rotate_velocity += forward * input.y; 

	// �O�̈ړ��ʂ��擾
	GSvector3 prev_velocity = velocity_;
	prev_velocity.y = 0.0f;
	prev_velocity = prev_velocity.normalized() * move_speed_ * delta_time;

	// �ړ��ʂ��X�V
	velocity_.x = prev_velocity.x;
	velocity_.z = prev_velocity.z;

	// ��ђʈړ�
	non_penetrating_move(prev_velocity, camera_->is_lockon() ? nullptr : &rotate_velocity, TURN_SPEED * delta_time);
}

void Player::to_move_state() {
	change_state((GSuint)PlayerStateType::Move, (GSuint)Motion::Idle, true);
}

void Player::update_lockon_camera() {
	// �w��̏�Ԃł̓��b�N�I���؂�ւ����g���Ȃ�
	if (MyLib::is_in(
		state_.get_current_state(),
		(GSuint)PlayerStateType::Idle,
		(GSuint)PlayerStateType::Dead
	)) return;
	
	// ���b�N�I�����͂���������
	if (input_.action(InputAction::GAME_Lockon)) {
		// ���b�N�I���^�[�Q�b�g������Ȃ烍�b�N�I��������
		if (camera_->is_lockon()) {
			camera_->set_lockon_target(nullptr);
			return;
		}
		// ���b�N�I���^�[�Q�b�g�����Ȃ���ΑΏۂ�T��
		else {
			Pawn* target = nullptr;
			// TODO search

			if (target != nullptr) {
				camera_->set_lockon_target(target);
				return;
			}
		}
	}
}

void Player::on_attack() {
	// TODO
}

void Player::on_avoid() {
	invincible_timer_ = INVINCIBLE_TIME;

	// TODO
	// ���b�N�I�����Ȃ�4�������[�V������K�p����
	if (camera_->is_lockon()) {

	}
	// ���b�N�I�����łȂ����1�������[�V������K�p����
	else {

	}
}

void Player::on_skill() {
	// TODO
}

void Player::on_interact() {
	// TODO
}

bool Player::is_attack() const {
	return input_.action(InputAction::GAME_Attack);
}

bool Player::is_jump() const {
	return input_.action(InputAction::GAME_Jump);
}

bool Player::is_avoid() const {
	return input_.action(InputAction::GAME_Avoid);
}

bool Player::is_skill() const {
	return input_.action(InputAction::GAME_Skill);
}

bool Player::is_interact() const {
	return input_.action(InputAction::GAME_Interact);
}

GSuint Player::get_attack_motion() const {
	return (GSuint)Motion::Attack;
}

GSuint Player::get_avoid_motion() const {
	return (GSuint)Motion::Avoid;
}

GSuint Player::get_skill_motion() const {
	return (GSuint)Motion::Skill;
}

void Player::on_air() {
	if (MyLib::is_in(
		state_.get_current_state(),
		(GSuint)PlayerStateType::Idle,
		(GSuint)PlayerStateType::Hurt,
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Jump,
		(GSuint)PlayerStateType::Fall
	)) return;

	change_state((GSuint)PlayerStateType::Fall, Motion::Fall, true);
}

void Player::on_ground() {
	if (is_ground_) return;
	if (MyLib::is_in(
		state_.get_current_state(),
		(GSuint)PlayerStateType::Idle,
		(GSuint)PlayerStateType::Hurt,
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Land
	)) return;

	change_state((GSuint)PlayerStateType::Land, Motion::Land, false);
}
