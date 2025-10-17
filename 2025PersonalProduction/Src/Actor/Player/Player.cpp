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
#include "Engine/Core/Setting/Setting.h"
#endif

#include "Engine/Graphics/Canvas/Canvas.h"	// tmp

#include "GUI/InteractUI.h"

// 衝突判定用の半径
const float RADIUS{ 0.4f };
// 移動時のカメラ向きへの回転角度
const float TURN_SPEED{ 11.5f };
// 通常移動速度
const float MOVE_SPEED{ 0.065f };
// 疾走移動速度
const float SPRINT_SPEED{ 0.085f };
// 減速移動速度倍率
const float DECELERATION_SPEED{ 0.75f };
// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.5f };
// 回避移動速度
const float AVOID_SPEED{ 12.0f };

// 回避演出の時間
const float AVOID_EFFECT_TIME{ 3.0f };
// 回避演出の色
const GScolor AVOID_EFFECT_COLOR{ 0.592f, 0.627f, 1.0f, 1.0f };

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
	
	// 衝突判定球を生成
	collider_ = BoundingSphere{ RADIUS, GSvector3{ 0.0f, height_ / 2.0f, 0.0f } };

	transform_.position(position);
	transform_.lookAt(lookat);
	collide_field();
	mesh_.transform(transform_.localToWorldMatrix());

	// 攻撃アニメーションイベント
	add_attack_animation_event();
}

void Player::update(float delta_time) {
	update_invincible(delta_time);
	update_state(delta_time);
	update_gravity(delta_time);
	update_mesh(delta_time);

	// 回避演出タイマーの更新
	if (avoid_effect_timer_ > 0.0f) {
		avoid_effect_timer_ -= delta_time / cFPS;
		if (avoid_effect_timer_ <= 0.0f) Tween::color(AVOID_EFFECT_COLOR, GScolor{1.0f, 1.0f, 1.0f, 1.0f }, 0.5f * cFPS, [=](GScolor color) {
				world_->set_mask_color(color);
			}).on_complete([=] { world_->enable_avoid_effect() = false; });
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
    ImGui::SliderInt("right hand bone", &right_hand_bone_, 0, 255);
    int motion = motion_;
    ImGui::SliderInt("motion",&motion, 0, 255);
    motion_ = motion;
	ImGui::End();

	ImGui::Begin("App Window");
	const ScreenData& screen = Screen::get_instance().get_current_data();
	ImGui::Text("fps %f", delta_time * screen.refresh_rate);
	ImGui::Text("width %d height %d", screen.width_px, screen.height_px);
	std::string input_type = "input ";
	if (input_.is_pad()) input_type += "gamepad";
	else input_type += "keyboard + mouse";
	ImGui::Text("%s", input_type.c_str());

    Setting& setting = Setting::get_instance();
    std::string ssao = "ssao: ";
    ssao += setting.is_draw_ssao() ? "on" : "off";
    if (ImGui::Button(ssao.c_str())) setting.enable_draw_ssao() = !setting.enable_draw_ssao();

    std::string bloom = "bloom: ";
    bloom += setting.is_draw_bloom() ? "on" : "off";
    if (ImGui::Button(bloom.c_str())) setting.enable_draw_bloom() = !setting.enable_draw_bloom();

    std::string fxaa = "fxaa: ";
    fxaa += setting.is_draw_fxaa() ? "on" : "off";
    if (ImGui::Button(fxaa.c_str())) setting.enable_draw_fxaa() = !setting.enable_draw_fxaa();

	ImGui::End();
#endif

    if (gsGetKeyTrigger(GKEY_7)) {
        world_->game_save_data().inventory().add(ItemType::Weapon, 1, 1);
    }
}

void Player::late_update(float delta_time) {
    interact_update();
}

void Player::draw() const {
	mesh_.draw();

    // 武器の描画
    Inventory& inv = world_->game_save_data().inventory();
    if (!inv.weapon().is_empty()) {
        glPushMatrix();
        glMultMatrixf(mesh_.bone_matrices(right_hand_bone_));   // 右手に持たせる
        gsDrawMesh(item_data_.get_weapon(inv.weapon().id).mesh);
        glPopMatrix();
    }

	// state_.draw();
}

void Player::draw_gui() const {
	{
		const GSrect rect{ 0.0f, 0.0f, 77.0f, 80.0f };
		Canvas::draw_texture((GSuint)TextureID::TmpUI, GSvector2{ 20.0f, 20.0f }, rect, GSvector2::zero(), GSvector2::one(), GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, Anchor::TopLeft);
	}

    // インタラクトUIの描画
    InteractUI::draw(interact_actors_, interact_target_index_);

	// state_.draw_gui();
}

void Player::take_damage(Actor& other, const int damage) {
	if (MyLib::is_in(
		state_.get_current_state(), 
		(GSuint)PlayerStateType::Idle, 
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Skill	// スキル中も無敵
	)) return;
	if (invincible_timer() > 0.0f) {
		// 回避演出
		if (state_.get_current_state() == (GSuint)PlayerStateType::Avoid) {
			world_->enable_avoid_effect() = true;
			world_->set_mask_color(AVOID_EFFECT_COLOR);
			avoid_effect_timer_ = AVOID_EFFECT_TIME;
		}
		return;
	}

	//hp_ = CLAMP(hp_ - damage, 0, INT_MAX);

	if (hp_ <= 0) {
		change_state((GSuint)PlayerStateType::Dead, Motion::Dead, false);
	}
	else {
		// コライダーの位置から負傷モーションを取得
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
	// カメラ基準の方向を取得
	Camera* camera = world_->get_camera();
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// 入力から移動ベクトルを算出
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	const GSvector2 input = input_.left_axis();
	velocity += right * input.x;
	velocity += forward * input.y;
	
	// 歩行か疾走かを取得
	const bool is_walk = input_.action(InputAction::GAME_Sprint);

	// ロックオン中かどうかを取得
	const bool is_lockon = camera_->is_lockon();

	// 移動していたら移動量を、移動していなかったら減速するように移動量を計算
	if (velocity.magnitude() > 0.01f) {
		move_speed_ = is_walk ? SPRINT_SPEED : MOVE_SPEED;
		velocity = velocity.normalized() * move_speed_ * delta_time;
	}
	else {
		velocity.x = velocity_.x;
		velocity.z = velocity_.z;
		// 減速
		if (velocity.magnitude() > 0.01f) velocity *= DECELERATION_SPEED * delta_time;	// delta_time後計算での多少の誤差は気にしないものとする
		// 停止
		else velocity = GSvector3{ 0.0f, 0.0f, 0.0f };
	}
	// 移動量を更新
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;

	// 非貫通移動
	non_penetrating_move(velocity, is_lockon ? &forward : &velocity, TURN_SPEED * delta_time);

	// 移動状態でなければ終了
	if (!(state_.get_current_state() == (GSuint)PlayerStateType::Move)) return;
	// モーションを設定
	GSuint motion = Motion::Idle;
	if (input.magnitude() > 0.01f) {
		// ロックオン中なら8方向モーションを適用する
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
		// ロックオン中でなければ1方向モーションを適用する
		else {
			motion = is_walk ? Motion::WalkF : Motion::SprintF;
		}
	}
	// モーションを適用
	mesh_.change_motion(motion, true);
}

void Player::update_move_air(float delta_time) {
	// カメラ基準の方向を取得
	Camera* camera = world_->get_camera();
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// 入力から回転ベクトルを算出
	GSvector3 rotate_velocity{ 0.0f, 0.0f, 0.0f };
	GSvector2 input = input_.left_axis();
	rotate_velocity += right * input.x;
	rotate_velocity += forward * input.y; 

	// 前の移動量を取得
	GSvector3 prev_velocity = velocity_;
	prev_velocity.y = 0.0f;
	prev_velocity = prev_velocity.normalized() * move_speed_ * delta_time;

	// 移動量を更新
	velocity_.x = prev_velocity.x;
	velocity_.z = prev_velocity.z;

	// 非貫通移動
	non_penetrating_move(prev_velocity, camera_->is_lockon() ? nullptr : &rotate_velocity, TURN_SPEED * delta_time);
}

void Player::to_move_state() {
	change_state((GSuint)PlayerStateType::Move, (GSuint)Motion::Idle, true);
    move_speed_ = 0.0f;
}

void Player::update_lockon_camera() {
	// 指定の状態ではロックオン切り替えを使えない
	if (MyLib::is_in(
		state_.get_current_state(),
		(GSuint)PlayerStateType::Idle,
		(GSuint)PlayerStateType::Dead
	)) return;
	
	// ロックオン入力があったら
	if (input_.action(InputAction::GAME_Lockon)) {
		// ロックオンターゲットがいるならロックオンを解除
		if (camera_->is_lockon()) {
			camera_->set_lockon_target(nullptr);
			return;
		}
		// ロックオンターゲットがいなければ対象を探す
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
	// 攻撃段数を一段階目にセット
	attack_count_ = 1;
}

void Player::on_avoid() {
	// 一定時間無敵にする
	invincible_timer_ = INVINCIBLE_TIME;

	// カメラ基準の方向を取得
	Camera* camera = world_->get_camera();
	GSvector3 forward = camera != nullptr ? camera->transform().forward() : GSvector3{ 0.0f, 0.0f, 1.0f };
	forward.y = 0.0f;
	GSvector3 right = camera != nullptr ? camera->transform().right() : GSvector3{ -1.0f, 0.0f, 0.0f };
	right.y = 0.0f;

	// ロックオン中かどうかを取得
	const bool is_lockon =camera_->is_lockon();

	// 入力から回避ベクトルを算出
	GSvector3 avoid_velocity{ 0.0f, 0.0f, 0.0f };
	const GSvector2 input = input_.left_axis();
	// ロックオン中なら四方向に回避する
	if (is_lockon) {
		// 左右を優先する
		avoid_velocity += right * input.x;
		// 左右入力が無ければ前後に回避する
		if (avoid_velocity.magnitude() < 0.01f) avoid_velocity += forward * input.y;
	}
	// 非ロックオンなら自由に回避する
	else {
		avoid_velocity += right * input.x;
		avoid_velocity += forward * input.y;
	}
	// 入力が無ければ後退とする
	if (avoid_velocity.magnitude() < 0.01f) avoid_velocity += forward * -1.0f;

	// モーションを設定
	GSuint motion = Motion::AvoidB;
	// ロックオン中なら4方向モーションを適用する
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
	// ロックオン中でなければ1方向モーションを適用する
	else {
		motion = Motion::AvoidF;
	}

	// 移動先を決定
	avoid_velocity = avoid_velocity.normalized() * AVOID_SPEED * 1.0f / cFPS;
	// モーションを適用
	change_state((GSuint)PlayerStateType::Avoid, motion, false);

	// 移動
	float move_time = mesh_.motion_end_time();
	Tween::vector3(avoid_velocity, GSvector3::zero(), move_time, [&](GSvector3 pos) {
		non_penetrating_move(pos); }).ease(EaseType::EaseOutCubic);
	// 強制回転
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
	return weapon_manager_.get_enter_next_animation_time(world_->game_save_data().inventory().weapon().type, attack_count_);
}

bool Player::is_attack() {
	return attack_count_ < weapon_manager_.get_max_attack_count(world_->game_save_data().inventory().weapon().type) && input_.action(InputAction::GAME_Attack);
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
	int motion = weapon_manager_.get_animation_num(world_->game_save_data().inventory().weapon().type, attack_count_);
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
	GSvector3 local_pos = weapon_manager_.get_collider_offset(world_->game_save_data().inventory().weapon().type, attack_count_);
	GSmatrix4 m = local_to_world(local_pos, GSvector3::zero(), GSvector3::one());

    WeaponData::Data weapon =  world_->game_save_data().inventory().weapon();
    int damage = weapon.is_empty() ? 0 : weapon.damage;

	world_->generate_attack_collider(0.3f, m.position(), this, damage, 0.1f, 0.0f);
}

void Player::interact_update() {
    // インタラクトできるアクターを集める
    get_interact_actor_list();

    // 対象を選択
    if (interact_actors_.empty()) {
        interact_target_index_ = 0;
        return;
    }
    int target_index = (int)interact_target_index_;
    if (input_.action(InputAction::GAME_Interact_Up)) --target_index;
    else if (input_.action(InputAction::GAME_Interact_Down)) ++target_index;
    interact_target_index_ = CLAMP(target_index, 0, interact_actors_.size() - 1);

    // インタラクト
    if(input_.action(InputAction::GAME_Interact)) {
        if (interact_target_index_ <= 0) return;
        std::any data = this;
        interact_actors_[interact_target_index_]->message("PlayerInteract", data);
    }
}

void Player::get_interact_actor_list() {
    // 初期化
    interact_actors_.clear();

    for (Actor* actor : world_->get_all_actor()) {
        // インタラクトできないなら次
        if (!actor->can_interact()) continue;
        // 死んでるなら次
        if (actor->is_dead()) continue;
        // 距離外なら次
        float r = (transform_.position() - actor->transform().position()).magnitude();
        if (r > 1.5f) continue;
        // 対象として追加
        interact_actors_.push_back(actor);
    }
}

void Player::add_attack_animation_event() {
	// TODO 外部ファイル読み込みにする player_weapon_data.json ?

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
	WeaponType type = WeaponType::Sword;
	std::vector<WeaponManager::WeaponAnimationData*> data;
	
	// for2 start // TODO ***仮でそのまま記述***
	// 1段目
	data.push_back(new WeaponManager::WeaponAnimationData(34, 20, GSvector3{ 0.0f, 1.0f, 1.0f }, 30.0f));
	add_event(34, 20);
	// 2段目
	data.push_back(new WeaponManager::WeaponAnimationData(35, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 30.0f));
	add_event(35, 15);
	// 3段目
	data.push_back(new WeaponManager::WeaponAnimationData(36, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 30.0f));
	add_event(36, 15);
	// 4段目
	data.push_back(new WeaponManager::WeaponAnimationData(37, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 20.0f));
	add_event(37, 15);
    // 5段目
    data.push_back(new WeaponManager::WeaponAnimationData(38, 15, GSvector3{ 0.0f, 1.0f, 1.0f }, 20.0f));
    add_event(38, 15);
	// for2 end

	// 追加
	weapon_manager_.add_weapon_parameter(type, data);
	// for1 end
}

bool Player::is_root_motion_state() const {
    return false;
    //return MyLib::is_in(state_.get_current_state(),
    //    (GSuint)PlayerStateType::Attack
    //);
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

void Player::update_mesh(float delta_time) {
    // メッシュのモーションを更新
    mesh_.update(delta_time);
    // ルートモーションを適用
    if (is_root_motion_state()) mesh_.apply_root_motion(transform_);
    // ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}
