#include "Actor/Player/Player.h"
#include "Assets.h"
#include "Engine/Core/World/IWorld.h"
#include "State/Player/PlayerState.h"
#include "Engine/Core/Camera/Camera.h"
#include "Camera/PlayerCamera.h"
#include "Engine/Utils/Check.h"
#include "Engine/Utils/Calc.h"
#include "Engine/Core/Tween/Tween.h"
#include "GameConfig.h"

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
#include "Engine/Core/Screen/Screen.h"
#endif

#include "Engine/Graphics/Canvas/Canvas.h"	// tmp

// �Փ˔���p�̔��a
const float RADIUS{ 0.4f };
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
// ����ړ����x
const float AVOID_SPEED{ 12.0f };

// ������o�̎���
const float AVOID_EFFECT_TIME{ 3.0f };
// ������o�̐F
const GSvector3 AVOID_EFFECT_COLOR{ 0.592f, 0.627f, 1.0f };

Player::Player(IWorld* world, const GSvector3& position, const GSvector3& lookat, PlayerCamera* camera) {
	world_ = world;
	tag_ = ActorTag::Player;
	name_ = "Player";

	camera_ = camera;
	camera_->set_owner(this);

	height_ = 1.6f;
	head_offset_ = height_;
	foot_offset_ = 0.05f;

	mesh_ = { (GSuint)MeshID::Player, (GSuint)MeshID::Player, (GSuint)MeshID::Player };
	add_state();
	change_state((GSuint)PlayerStateType::Move, Motion::Idle, true);
	
	// �Փ˔��苅�𐶐�
	collider_ = BoundingSphere{ RADIUS, GSvector3{ 0.0f, height_ / 2.0f, 0.0f } };

	transform_.position(position);
	transform_.lookAt(lookat);
	collide_field();
	mesh_.transform(transform_.localToWorldMatrix());

	// �U���A�j���[�V�����C�x���g
	add_attack_animation_event();
}

void Player::update(float delta_time) {
	update_invincible(delta_time);
	update_state(delta_time);
	update_gravity(delta_time);
	update_mesh(delta_time);

	// ������o�^�C�}�[�̍X�V
	if (avoid_effect_timer_ > 0.0f) {
		avoid_effect_timer_ -= delta_time / cFPS;
		if (avoid_effect_timer_ <= 0.0f) Tween::vector3(AVOID_EFFECT_COLOR, GSvector3::one(), 0.5f * cFPS, [=](GSvector3 color) {
				world_->set_avoid_effect_color(color);
			}).on_complete([=] { world_->enable_avoid_posteffct() = false; });
	}

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
	ImGui::Text("current motion is %d.", (int)motion_);
	ImGui::End();

	ImGui::Begin("App Window");
	const ScreenData& screen = Screen::get_instance().get_current_data();
	ImGui::Text("fps %f", delta_time * screen.refresh_rate);
	ImGui::Text("width %d height %d", screen.width_px, screen.height_px);
	std::string input_type = "input ";
	if (input_.is_pad()) input_type += "gamepad";
	else input_type += "keyboard + mouse";
	ImGui::Text("%s", input_type.c_str());
	ImGui::End();
#endif
}

void Player::late_update(float delta_time) {

	// TODO IK
}

void Player::draw() const {
	mesh_.draw();

	// state_.draw();
}

void Player::draw_gui() const {
	// TODO GUI

	{
		const GSrect rect{ 0.0f, 0.0f, 77.0f, 80.0f };
		Canvas::draw_texture((GSuint)TextureID::TmpUI, GSvector2{ 20.0f, 20.0f }, rect, GSvector2::zero(), GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::TopLeft);
	}

	// state_.draw_gui();
}

void Player::take_damage(Actor& other, const int damage) {
	if (MyLib::is_in(
		state_.get_current_state(), 
		(GSuint)PlayerStateType::Idle, 
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Skill	// �X�L���������G
	)) return;
	if (invincible_timer() > 0.0f) {
		// ������o
		if (state_.get_current_state() == (GSuint)PlayerStateType::Avoid) {
			world_->enable_avoid_posteffct() = true;
			world_->set_avoid_effect_color(AVOID_EFFECT_COLOR);
			avoid_effect_timer_ = AVOID_EFFECT_TIME;
		}
		return;
	}

	//hp_ = CLAMP(hp_ - damage, 0, INT_MAX);

	if (hp_ <= 0) {
		change_state((GSuint)PlayerStateType::Dead, Motion::Dead, false);
	}
	else {
		// �R���C�_�[�̈ʒu���畉�����[�V�������擾
		const GSvector3 other_dir = other.transform().position() - transform().position();
		const int dir = MyLib::get_direction(GSvector2{ other_dir.x, other_dir.z }, GSvector2{ transform().forward().x, transform().forward().z}, 4);
		GSuint motion = Motion::HurtF;
		switch (dir) {
		case 0: motion = Motion::HurtF; break;
		case 1: motion = Motion::HurtR; break;
		case 2: motion = Motion::HurtB; break;
		case 3: motion = Motion::HurtL; break;
		default: break;
		}

		change_state((GSuint)PlayerStateType::Hurt, motion, false);
	}

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
	// �J������̕������擾
	Camera* camera = world_->get_camera();
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// ���͂���ړ��x�N�g�����Z�o
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	const GSvector2 input = input_.left_axis();
	velocity += right * input.x;
	velocity += forward * input.y;
	
	// ���s�����������擾
	const bool is_walk = input_.action(InputAction::GAME_Sprint);

	// ���b�N�I�������ǂ������擾
	const bool is_lockon = camera_->is_lockon();

	// �ړ����Ă�����ړ��ʂ��A�ړ����Ă��Ȃ������猸������悤�Ɉړ��ʂ��v�Z
	if (velocity.magnitude() > 0.01f) {
		move_speed_ = is_walk ? SPRINT_SPEED : MOVE_SPEED;
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
	non_penetrating_move(velocity, is_lockon ? &forward : &velocity, TURN_SPEED * delta_time);

	// �ړ���ԂłȂ���ΏI��
	if (!(state_.get_current_state() == (GSuint)PlayerStateType::Move)) return;
	// ���[�V������ݒ�
	GSuint motion = Motion::Idle;
	if (input.magnitude() > 0.01f) {
		// ���b�N�I�����Ȃ�8�������[�V������K�p����
		if (is_lockon) {
			const int dir = MyLib::get_direction(GSvector2{ velocity.x, velocity.z }, GSvector2{ forward.x, forward.z }, 8);
			switch (dir) {
			case 0: motion = is_walk ? Motion::WalkF : Motion::SprintF; break;
			case 1: motion = is_walk ? Motion::WalkFL : Motion::SprintFL; break;
			case 2: motion = is_walk ? Motion::WalkL : Motion::SprintL; break;
			case 3: motion = is_walk ? Motion::WalkBL : Motion::SprintBL; break;
			case 4: motion = is_walk ? Motion::WalkB : Motion::SprintB; break;
			case 5: motion = is_walk ? Motion::WalkBR : Motion::SprintBR; break;
			case 6: motion = is_walk ? Motion::WalkR : Motion::SprintR; break;
			case 7: motion = is_walk ? Motion::WalkFR : Motion::SprintFR; break;
			default: break;
			}
		}
		// ���b�N�I�����łȂ����1�������[�V������K�p����
		else {
			motion = is_walk ? Motion::WalkF : Motion::SprintF;
		}
	}
	// ���[�V������K�p
	mesh_.change_motion(motion, true);
}

void Player::update_move_air(float delta_time) {
	// �J������̕������擾
	Camera* camera = world_->get_camera();
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
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
	// �U���i������i�K�ڂɃZ�b�g
	attack_count_ = 1;
}

void Player::on_avoid() {
	// ��莞�Ԗ��G�ɂ���
	invincible_timer_ = INVINCIBLE_TIME;

	// �J������̕������擾
	Camera* camera = world_->get_camera();
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// ���b�N�I�������ǂ������擾
	const bool is_lockon =camera_->is_lockon();

	// ���͂������x�N�g�����Z�o
	GSvector3 avoid_velocity{ 0.0f, 0.0f, 0.0f };
	const GSvector2 input = input_.left_axis();
	// ���b�N�I�����Ȃ�l�����ɉ������
	if (is_lockon) {
		// ���E��D�悷��
		avoid_velocity += right * input.x;
		// ���E���͂�������ΑO��ɉ������
		if (avoid_velocity.magnitude() < 0.01f) avoid_velocity += forward * input.y;
	}
	// �񃍃b�N�I���Ȃ玩�R�ɉ������
	else {
		avoid_velocity += right * input.x;
		avoid_velocity += forward * input.y;
	}
	// ���͂�������Ό�ނƂ���
	if (avoid_velocity.magnitude() < 0.01f) avoid_velocity += forward * -1.0f;

	// ���[�V������ݒ�
	GSuint motion = Motion::AvoidB;
	// ���b�N�I�����Ȃ�4�������[�V������K�p����
	if (is_lockon) {
		const int dir = MyLib::get_direction(GSvector2{ avoid_velocity.x, avoid_velocity.z }, GSvector2{ forward.x, forward.z }, 4);
		switch (dir) {
		case 0: motion = Motion::AvoidF; break;
		case 3: motion = Motion::AvoidR; break;
		case 2: motion = Motion::AvoidB; break;
		case 1: motion = Motion::AvoidL; break;
		default: break;
		}
	}
	// ���b�N�I�����łȂ����1�������[�V������K�p����
	else {
		motion = Motion::AvoidF;
	}

	// �ړ��������
	avoid_velocity = avoid_velocity.normalized() * AVOID_SPEED * 1.0f / cFPS;
	// ���[�V������K�p
	change_state((GSuint)PlayerStateType::Avoid, motion, false);

	// �ړ�
	float move_time = mesh_.motion_end_time();
	Tween::vector3(avoid_velocity, GSvector3::zero(), move_time, [&](GSvector3 pos) {
		non_penetrating_move(pos); }).ease(EaseType::EaseOutCubic);
	// ������]
	transform_.lookAt(transform_.position() + (is_lockon ? forward : avoid_velocity));

	velocity_.x = 0.0f;
	velocity_.z = 0.0f;
}

void Player::on_skill() {
	// TODO
}

void Player::on_interact() {
	// TODO
}

int& Player::attack_count() {
	return attack_count_;
}

float Player::get_enter_next_attack_animation_time() {
	return weapon_manager_.get_enter_next_animation_time(weapon_type_, attack_count_);
}

bool Player::is_attack() {
	return attack_count_ < weapon_manager_.get_max_attack_count(weapon_type_) && input_.action(InputAction::GAME_Attack);
}

bool Player::is_jump() const {
	return state_.get_current_state() != (GSuint)PlayerStateType::Jump && input_.action(InputAction::GAME_Jump);
}

bool Player::is_avoid() const {
	return state_.get_current_state() != (GSuint)PlayerStateType::Avoid && input_.action(InputAction::GAME_Avoid);
}

bool Player::is_skill() const {
	return state_.get_current_state() != (GSuint)PlayerStateType::Skill && input_.action(InputAction::GAME_Skill);
}

bool Player::is_interact() const {
	return input_.action(InputAction::GAME_Interact);
}

GSuint Player::get_attack_motion() {
	int motion = weapon_manager_.get_animation_num(weapon_type_, attack_count_);
	return motion < 0 ? 99999 : motion;
}

GSuint Player::get_skill_motion() const {
	return (GSuint)Motion::Skill;
	// TODO weapon num
}

GSuint Player::get_current_motion() const {
	return motion_;
}

void Player::generate_attack_collider() {
	GSvector3 local_pos = weapon_manager_.get_collider_offset(weapon_type_, attack_count_);
	GSmatrix4 m = local_to_world(local_pos, GSvector3::zero(), GSvector3::one());

	// TODO damage
	world_->generate_attack_collider(0.3f, m.position(), this, 1, 0.1f, 0.0f);
}

void Player::add_attack_animation_event() {
	// TODO �O���t�@�C���ǂݍ��݂ɂ��� player_weapon_data.json ?

	// mabye key = weapon type
	// value = [
	//	   {	"animation num": 0,
	//			"key_frame": 20,
	//			"offset": [0.0, 0.0, 1.0]
	//     }
	// ]

	auto add_event = [this](int num, float time) {
		mesh_.add_animation_event(num, time, [=] {generate_attack_collider(); });
	};

	// for1 start
	WeaponType type = WeaponType::PlayerSword;
	std::vector<WeaponManager::WeaponAnimationData*> data;
	
	// for2 start // TODO ***���ł��̂܂܋L�q***
	// 1�i��
	data.push_back(new WeaponManager::WeaponAnimationData(0, 20, GSvector3{ 0.0f, 1.0f, 1.0f }, 30.0f));
	add_event(0, 20);
	// 2�i��
	data.push_back(new WeaponManager::WeaponAnimationData(1, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 30.0f));
	add_event(1, 15);
	// 3�i��
	data.push_back(new WeaponManager::WeaponAnimationData(2, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 30.0f));
	add_event(2, 15);
	// 4�i��
	data.push_back(new WeaponManager::WeaponAnimationData(3, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 20.0f));
	add_event(3, 15);
	// for2 end

	// �ǉ�
	weapon_manager_.add_weapon_parameter(type, data);
	// for1 end
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
