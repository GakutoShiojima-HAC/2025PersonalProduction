#include "Actor/Player/Player.h"
#include "Assets.h"
#include "GameConfig.h"
#include "Camera/PlayerCamera.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Core/Vibration/Vibration.h"
#include "Engine/Utils/Check.h"
#include "Engine/Utils/Calc.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Sound/SE.h"
#include "GUI/PlayerUI.h"
#include "GUI/InteractUI.h"
#include "Score/ActionScore.h"
#include "State/Player/PlayerState.h"

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
#include "Engine/Utils/MyString.h"
#endif

#include "Engine/Graphics/Canvas/Canvas.h"	// tmp

// 移動時のカメラ向きへの回転角度
constexpr float TURN_SPEED{ 18.0f };
// 空中移動時のカメラ向きへの回転角度
constexpr float AIR_TURN_SPEED{ 5.0f };
// 通常移動速度
constexpr float MOVE_SPEED{ 0.02f };
// 疾走移動速度
constexpr float SPRINT_SPEED{ 0.095f };
// 減速移動速度倍率
constexpr float DECELERATION_SPEED{ 0.75f };
// 無敵時間(秒)
constexpr float INVINCIBLE_TIME{ 0.5f };
// 回避移動速度
constexpr float AVOID_SPEED{ 10.0f };
// ジャンプ力
constexpr float JUMP_POWER{ 0.215f };

// 回避成功判定距離
constexpr float AVOID_MAX_RANGE{ 3.5f };
// 回避演出の時間
constexpr float AVOID_EFFECT_TIME{ 3.0f };
// 回避演出の色
const GScolor AVOID_EFFECT_COLOR{ 0.592f, 0.627f, 1.0f, 1.0f };
// 回避攻撃のクリティカルポイントからのオフセット
const GSvector3 AVOID_ATTACK_OFFSET{ 0.75f, 0.0f, 1.125f };

// 右手のボーン番号
constexpr int RIGHT_HAND_BONE_NUM{ 114 };

// 基礎体力
constexpr int BASIC_HP{ 10 };

constexpr float ATTACK_STEP_POWER{ 0.075f };

// 攻撃モーション
constexpr GSuint ATTACK_MOTION_MAX = 5;
constexpr GSuint ATTACK_MOTION[] = {
    Player::Motion::Attack1,
    Player::Motion::Attack2,
    Player::Motion::Attack3,
    Player::Motion::Attack4,
    Player::Motion::Attack5
};

Player::Player(IWorld* world, const GSvector3& position, const GSvector3& rotate, const PlayerInfo& info) :
    inventory_{ world->game_save_data().inventory() } {
	world_ = world;
	tag_ = ActorTag::Player;
	name_ = "Player";

    init_parameter(PawnParameter::get_type("Normal"));

    // タイムスケールを受けない
    enable_timescale_ = false;

    // HPを計算
    GameSaveData& save_data = world_->game_save_data();
    max_hp_ = hp_ = BASIC_HP + save_data.get().player_level * 2;
    display_hp_ = (float)max_hp_;

	mesh_ = { (GSuint)MeshID::Player, (GSuint)MeshID::Player, (GSuint)MeshID::Player };
	add_state();
	
	transform_.position(position);
    transform_.eulerAngles(rotate);
	mesh_.transform(transform_.localToWorldMatrix());
	change_state((GSuint)PlayerStateType::Move, Motion::Idle, true);
    collide_field();

    // カメラの追加
    camera_ = new PlayerCamera{ world };
    camera_->set_owner(this);
    world_->add_camera(camera_);
    world_->camera_transition(camera_);

    // 情報を保持
    attack_param_ = info.attack_param;

    // 足音
    auto play_step = [=](GSuint motion, float time) {
        mesh_.add_animation_event(motion, time, [=] { SE::play_random((GSuint)SEID::Step, 0.5f); });
    };
    play_step(Motion::WalkF, 8.0f);
    play_step(Motion::WalkF, 36.0f);
    play_step(Motion::SprintF, 3.0f);
    play_step(Motion::SprintF, 19.0f);

	// 攻撃アニメーションイベントを追加
	add_attack_animation_event(info);
}

void Player::update(float delta_time) {
    // 回避成功時は倍速
    if (world_->is_avoid_effect()) delta_time *= 1.2f;

	update_invincible(delta_time);
	update_state(delta_time);
    update_physics(delta_time);
    collide_field();
	update_mesh(delta_time);

#ifdef _DEBUG
	ImGui::Begin("App Window");
	const ScreenData& screen = Screen::get_instance().get_current_data();
	ImGui::Text("fps %d", (int)std::round(screen.refresh_rate / delta_time));
	ImGui::Text("width %d height %d", screen.width_px, screen.height_px);
    if (input_.is_pad()) ImGui::Text(ToUTF8("入力タイプ: ゲームパッド").c_str());
    else ImGui::Text(ToUTF8("入力タイプ: キーボード + マウス").c_str());

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

    std::string safearea = "safearea: ";
    safearea += setting.is_draw_safearea() ? "on" : "off";
    if (ImGui::Button(safearea.c_str())) setting.enable_draw_safearea() = !setting.enable_draw_safearea();

    std::string vibration = "vibration: ";
    vibration += setting.is_vibration() ? "on" : "off";
    if (ImGui::Button(vibration.c_str())) setting.enable_vibration() = !setting.enable_vibration();

	ImGui::End();
#endif
}

void Player::late_update(float delta_time) {
    update_display_hp(delta_time);
    update_interact();
}

void Player::draw() const {
	mesh_.draw();

    // 武器の描画
    if (!inventory_.weapon().is_empty() && draw_weapon_) {
        glPushMatrix();
        glMultMatrixf(mesh_.bone_matrices(RIGHT_HAND_BONE_NUM));   // 右手に持たせる
        gsDrawMesh(item_data_.get_weapon(inventory_.weapon().id).mesh);
        glPopMatrix();
    }

	// state_.draw();
}

void Player::draw_gui() const {
#ifdef _DEBUG
    Setting& setting = Setting::get_instance();
    if (setting.is_draw_safearea()) {
        gsDrawSprite2D((GSuint)TextureID::SafeArea, NULL, NULL, NULL, NULL, NULL, 0);
    }
#endif

    if (state_.get_current_state() == (GSuint)PlayerStateType::Idle) return;

    // インタラクトUIの描画
    InteractUI::draw(interact_actors_, interact_target_index_);

    // HPの描画
    PlayerUI::draw_hp(hp_, display_hp_, max_hp_);
    // レベルの描画
    PlayerUI::draw_level(world_->game_save_data().get().player_level);

	// state_.draw_gui();
}

#ifdef _DEBUG
void Player::debug_update(float delta_time) {
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
    // 名前
    std::string text = "name: " + name_;
    ImGui::Text(text.c_str());
    // 座標
    ImGui::Text("position: X:%.3f Y:%.3f Z:%.3f", transform_.position().x, transform_.position().y, transform_.position().z);
    // ステート番号
    ImGui::Text("state: %s", state_string(PlayerStateType(state_.get_current_state())));
    // モーション番号
    ImGui::Text("motion: %d", (int)motion_);
    // タイムスケール
    ImGui::InputFloat(ToUTF8("タイムスケール").c_str(), &world_->timescale(), 0.1f);
}
#endif

void Player::take_damage(Actor& other, const int damage) {
	if (MyLib::is_in(
		state_.get_current_state(), 
		(GSuint)PlayerStateType::Idle, 
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Skill	// スキル中も無敵
	)) return;
    if (invincible_timer() > 0.0f || world_->is_avoid_effect()) return;

	hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
	invincible_timer_ = INVINCIBLE_TIME;

	if (hp_ <= 0) {
		change_state((GSuint)PlayerStateType::Dead, Motion::Dead, false);
        return;
	}
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

void Player::on_hit_attack(AttackCollider& collider) {
    std::string name = collider.name();

    // 通常攻撃なら
    if (name == "PlayerNormalAttack") {
        // 基礎スコア 基礎値 + コンボ数 * ボーナス値 
        const int score = 50 + attack_count_ * 10;
        world_->action_score().add_score(score, mesh_.motion_end_time() * 1.25f, 0.125f); // 時間はモーション時間より少しだけ長めに
        // カメラを揺らす
        float strength = 2.5f + (0.5f * attack_count_);
        world_->camera_shake(CameraShakeType::Shake, 0.075f, strength, false);
        // コントローラーを振動させる
        if (input_.is_pad()) Vibration::get_instance().start(0.2f, 1.0f);
    }

    // 回避攻撃なら
    if (name == "PlayerAvoidAttack") {
        world_->action_score().add_score(100, mesh_.motion_end_time() * 2.0f, 0.5f);
        // カメラを揺らす
        world_->camera_shake(CameraShakeType::Shake, 0.125f, 4.0f, false);
        // コントローラーを振動させる
        if (input_.is_pad()) Vibration::get_instance().start(0.25f, 1.0f);
    }

    // 回避成功攻撃なら
    if (name == "PlayerAvoidSuccessAttack") {
        world_->action_score().add_score(250, mesh_.motion_end_time() * 5.0f, 1.5f);
        // カメラを揺らす
        world_->camera_shake(CameraShakeType::HandShake, 0.3f, 15.0f, false);
        // コントローラーを振動させる
        if (input_.is_pad()) Vibration::get_instance().start(0.25f, 1.0f);
        // インパクト
        GSvector2 screen;
        GSvector3 world_position = collider.collider().center;
        gsCalculateScreen(&screen, &world_position);
        world_->impact_effect_start(0.25f, screen);
    }
}

bool Player::is_dead_state() const {
	return MyLib::is_in(state_.get_current_state(), (GSuint)PlayerStateType::Dead) || is_dead_;
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
	
	// 歩行か疾走か
    const bool is_sprint = ABS(input.magnitude()) > 0.65f;

	// ロックオン中かどうかを取得
	const bool is_lockon = camera_->is_lockon();

	// 移動していたら移動量を、移動していなかったら減速するように移動量を計算
	if (velocity.magnitude() > 0.01f) {
		move_speed_ = is_sprint ? SPRINT_SPEED : MOVE_SPEED;
		velocity = velocity.normalized() * move_speed_ * delta_time;
	}
	else {
		velocity.x = prev_velocity_.x;
		velocity.z = prev_velocity_.z;
		// 減速
        if (velocity.magnitude() > 0.01f) {
            move_speed_ = move_speed_ * DECELERATION_SPEED;
            velocity = velocity.normalized() * move_speed_ * delta_time;
        }
        // 停止
        else {
            move_speed_ = 0.0f;
            velocity = GSvector3{ 0.0f, 0.0f, 0.0f };
        }
	}
	// 移動量を更新
    prev_velocity_.x = velocity.x;
    prev_velocity_.z = velocity.z;

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
			case 0: motion = is_sprint ? Motion::SprintF : Motion::WalkF; break;
			case 1: motion = is_sprint ? Motion::SprintFL : Motion::WalkFL; break;
			case 2: motion = is_sprint ? Motion::SprintL : Motion::WalkL; break;
            case 3: motion = is_sprint ? Motion::SprintBL : Motion::WalkBL; break;
			case 4: motion = is_sprint ? Motion::SprintB : Motion::WalkB; break;
			case 5: motion = is_sprint ? Motion::SprintBR : Motion::WalkBR; break;
			case 6: motion = is_sprint ? Motion::SprintR : Motion::WalkR; break;
			case 7: motion = is_sprint ? Motion::SprintFR : Motion::WalkFR; break;
			default: break;
			}
		}
		// ロックオン中でなければ1方向モーションを適用する
		else {
			motion = is_sprint ? Motion::SprintF : Motion::WalkF;
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
	GSvector3 prev_velocity = prev_velocity_;
	prev_velocity.y = 0.0f;
	prev_velocity = prev_velocity.normalized() * move_speed_ * delta_time;

	// 移動量を更新
    prev_velocity_.x = prev_velocity.x;
    prev_velocity_.z = prev_velocity.z;

	// 非貫通移動
	non_penetrating_move(prev_velocity, camera_->is_lockon() ? nullptr : &rotate_velocity, AIR_TURN_SPEED * delta_time);
}

void Player::to_move_state() {
    reset_move_speed();
	change_state((GSuint)PlayerStateType::Move, (GSuint)Motion::Idle, true);
}

void Player::reset_move_speed() {
    move_speed_ = 0.0f;
}

bool Player::is_move_input() const {
    return input_.left_axis().length() > 0.0f;
}

void Player::update_lockon_camera() {
	// ロックオン入力がなければ終了
    if (!input_.action(InputAction::GAME_Lockon)) return;

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

void Player::look_target() {
    // 一番近い敵を探す
    float min_length{ 3.0f };
    Pawn* target{ nullptr };
    const GSvector3 position = transform_.position();
    for (const auto& pawn : world_->find_pawn_with_tag(ActorTag::Enemy)) {
        // 死亡していたらスキップ
        if (pawn->is_dead_state()) continue;
        float length = (pawn->transform().position() - position).magnitude();
        // 一番近ければ対象を更新
        if (length < min_length) {
            min_length = length;
            target = pawn;
        }
    }
    if (target == nullptr) return;

    look_target(target);
}

void Player::look_target(Actor* target) {
    if (target == nullptr) return;

    const GSvector3 position = transform_.position();
    GSvector3 target_position = target->transform().position();
    target_position.y = position.y; // 向かせるだけなのでy座標は同じ
    transform_.lookAt(target_position);
}

void Player::stop_timeline() {
    world_->stop_timeline();
}

bool Player::is_action(InputAction action) const {
    switch (action) {
    case InputAction::GAME_Attack:
        // 入力があるかつ、攻撃段数が現在の武器最大攻撃可能数未満であれば真
        return input_.action(InputAction::GAME_Attack) && attack_count_ < ATTACK_MOTION_MAX && !inventory_.weapon().is_empty();
    case InputAction::GAME_Jump:
        return input_.action(InputAction::GAME_Jump) && state_.get_current_state() != (GSuint)PlayerStateType::Jump;
    case InputAction::GAME_Avoid:
        return input_.action(InputAction::GAME_Avoid) && state_.get_current_state() != (GSuint)PlayerStateType::Avoid;
    case InputAction::GAME_Skill:
        return input_.action(InputAction::GAME_Skill) && state_.get_current_state() != (GSuint)PlayerStateType::Skill;
    case InputAction::GAME_Interact:
        return input_.action(InputAction::GAME_Interact);
    default:
        return false;
    }
    return false;
}

void Player::attack_start() {
	// 攻撃段数を一段階目にセット
	attack_count_ = 0;
}

void Player::on_jump() {
    add_force(GSvector3{ 0.0f, JUMP_POWER, 0.0f }, Actor::ForceMode::Impulse);

    int handle = play_effect((GSuint)EffectID::OnGroundSmoke, GSvector3::zero());
    const GScolor color{ 1.0f, 1.0f, 1.0f, 0.25f };
    gsSetEffectColor(handle, &color);

    SE::play((GSuint)SEID::Jump);
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
	// 入力が無ければ前へ
	if (avoid_velocity.magnitude() < 0.01f) avoid_velocity += forward * 1.0f;

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

    const bool is_avoid_effect = world_->is_avoid_effect();
    const bool is_normal_hurt = state_.get_current_state() == (GSuint)PlayerStateType::Hurt && !is_avoid_effect;
    if (is_normal_hurt) {
        // ボーナス
        world_->action_score().add_score(100, mesh_.motion_end_time() * 5.0f, 0.5f);
        world_->action_score().set_action_text("緊急回避！");
    }

	// 移動先を決定
	avoid_velocity = avoid_velocity.normalized() * AVOID_SPEED / cFPS;
	// モーションを適用
	change_state((GSuint)PlayerStateType::Avoid, motion, false);

	// 移動
	float move_time = mesh_.motion_end_time();
	Tween::vector3(avoid_velocity, GSvector3::zero(), move_time, [&](GSvector3 pos) {
		non_penetrating_move(pos); }).ease(EaseType::EaseOutCubic).name("PlayerAvoid");
	// 強制回転
	transform_.lookAt(transform_.position() + (is_lockon ? forward : avoid_velocity));

    prev_velocity_.x = 0.0f;
    prev_velocity_.z = 0.0f;

    // 回避エフェクトを再生
    avoid_effect_handle_ = world_->camera_effect_play_foward((GSuint)EffectID::Avoid, 0.25f);
    const GScolor color{ 1.0f, 1.0f, 1.0f, 0.15f };
    gsSetEffectColor(avoid_effect_handle_, &color);
    SE::play_random((GSuint)SEID::Avoid, 0.25f);

    // 回避ターゲットをリセット
    avoid_target_ = nullptr;

    // 回避演出に入るかどうか(近くに攻撃動作に入っている敵がいるかどうか)
    if (is_avoid_effect) return;
    std::vector<Pawn*> enemys = world_->find_pawn_with_tag(ActorTag::Enemy);
    if (enemys.empty()) return;
    for (const auto& enemy : enemys) {
        //近くにいなければスキップ
        if ((enemy->transform().position() - transform_.position()).magnitude() > AVOID_MAX_RANGE) continue;
        // 攻撃動作に入っているか
        if (enemy->is_attack_soon()) {
            avoid_effect_start();   // 回避演出開始
            avoid_target_ = enemy;  // 回避ターゲットを保存
            break;
        }
    }
}

void Player::on_avoid_attack() {
    look_target();
    // 回避が成功していたら回避成功攻撃とする
    if (world_->is_avoid_effect()) {

        // ターゲットがいなければ近くの敵を探す
        if (avoid_target_ == nullptr) {
            // 一番近い敵を探す
            float min_length{ 6.0f };
            Pawn* target{ nullptr };
            const GSvector3 position = transform_.position();
            for (const auto& pawn : world_->find_pawn_with_tag(ActorTag::Enemy)) {
                // 死亡していたらスキップ
                if (pawn->is_dead_state()) continue;
                float length = (pawn->transform().position() - position).magnitude();
                // 一番近ければ対象を更新
                if (length < min_length) {
                    min_length = length;
                    target = pawn;
                }
            }
            avoid_target_ = target;
        }

        // ターゲットに対して攻撃する
        if (avoid_target_ != nullptr) {
            const GSvector3 world_offset = transform_.rotation() * AVOID_ATTACK_OFFSET;
            const float current_y = transform_.position().y;
            const GSvector3 critical = avoid_target_->critical_position().position();
            GSvector3 position = critical - world_offset;
            position.y = current_y;
            transform_.position(position);
            look_target(avoid_target_);
            collide_field();
        }

        change_state((GSuint)PlayerStateType::Skill, Motion::AvoidSuccessAttack, false);
        if (!world_->is_playing_timeline()) world_->play_timeline("AvoidSuccessAttack");
    }
    else {
        change_state((GSuint)PlayerStateType::Skill, Motion::AvoidAttack, false);
        if (!world_->is_playing_timeline()) world_->play_timeline("AvoidAttack");
    }

    // 回避ターゲットを開放
    avoid_target_ = nullptr;
}

void Player::on_skill() {
    look_target();
	// TODO
}

int& Player::attack_count() {
	return attack_count_;
}

float Player::get_enter_next_attack_min_time() const {
    if (attack_count_ >= attack_param_.size() - 1) return 99999.0f;
    return attack_param_[attack_count_].next_start;
}

float Player::get_enter_next_attack_max_time() const {
    if (attack_count_ >= attack_param_.size() - 1) return 99999.0f;
    return attack_param_[attack_count_].next_end;
}


void Player::set_draw_weapon(bool enabled) {
    // 描画が切り替わったら
    if (draw_weapon_ != enabled) {
        // TODO effect?
    }

    draw_weapon_ = enabled;
}

GSuint Player::get_attack_motion() const {
    return attack_count_ < ATTACK_MOTION_MAX ? ATTACK_MOTION[attack_count_] : 99999;
}

GSuint Player::get_skill_motion() const {
	return Motion::Skill;
	// TODO weapon num
}

GSuint Player::get_current_motion() const {
	return motion_;
}

void Player::avoid_effect_start() {
    invincible_timer_ = INVINCIBLE_TIME;

    // 回避演出開始
    world_->start_avoid_effect(3.0f, 0.25f);
    SE::play((GSuint)SEID::AvoidEffectStart);
    gsSetEffectSpeed(avoid_effect_handle_, 1.0f / 0.25f); // タイムスケールを受けないようにする
    enable_timescale_ = false;

    // ボーナス
    world_->action_score().add_score(250, mesh_.motion_end_time() * 5.0f, 2.0f);
    world_->action_score().set_action_text("回避成功！");
    // コントローラーを振動させる
    if (input_.is_pad()) Vibration::get_instance().start(0.25f, 1.0f);
}

void Player::update_interact() {
    // インタラクトさせないステート
    if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)PlayerStateType::Idle,
        (GSuint)PlayerStateType::Avoid,
        (GSuint)PlayerStateType::Dead,
        (GSuint)PlayerStateType::Jump,
        (GSuint)PlayerStateType::Fall,
        (GSuint)PlayerStateType::Skill
    )) {
        // クリアだけして終了
        interact_actors_.clear();
        return;
    }

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

    // インタラクトできないステート
    if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)PlayerStateType::Attack
    ))  return;

    // インタラクト
    if (input_.action(InputAction::GAME_Interact)) {
        std::any data = this;
        interact_actors_[interact_target_index_]->message("PlayerInteract", data);

        // アニメーション時間を適当なモーションを挟んでリセット
        mesh_.change_motion(Motion::Idle, false);
        change_state((GSuint)PlayerStateType::Interact, Motion::Interact, false);
        return;
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

    for (Actor* actor : world_->get_field()->get_all_actor()) {
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

void Player::add_attack_animation_event(const PlayerInfo& info) {
    // 対象のモーションのときに生成する判定
    auto set_generate_collider = [&](GSuint motion, const std::string& name, const std::vector<PlayerGenerateAttackColliderEvent>& events, const GSvector3& external_velocity) {
        for (const auto& param : events) {
            // イベントを登録
            mesh_.add_animation_event(motion, param.time, [=] { generate_attack_collider(
                param.offset, param.radius, info.skill_damage, name, external_velocity
                );
            });
        }
    };

    // 対象のモーションのときに生成するエフェクト
    auto set_effect_event = [&](GSuint motion, const std::vector<PlayerGenerateEffectEvent>& events, const bool is_move_foward = false) {
        for (const auto& param : events) {
            if (!param.enabled) continue;
            // イベントを登録
            mesh_.add_animation_event(motion, param.time, [=] {
                play_effect(param.effect_id, param.offset, param.rotate, param.scale, param.speed);
                SE::play_random((GSuint)SEID::Swing, 0.125f);
            });
            // イベントを登録
            if (is_move_foward) {
                mesh_.add_animation_event(motion, param.time, [=] {
                    add_force(transform_.forward() * ATTACK_STEP_POWER, Actor::ForceMode::Impulse);
                });
            }
        }
    };

    // 通常攻撃のアニメーションイベントを追加
    for (GSuint i = 0; i < ATTACK_MOTION_MAX; ++i) {
        if (info.attack_event.size() <= i) break;
        GSuint motion = ATTACK_MOTION[i];
        // 対象のモーションのときに生成する判定
        for (const auto& param : info.attack_event[i]) {
            // イベントを登録
            mesh_.add_animation_event(motion, param.time, [=] { generate_attack_collider(
                param.offset, param.radius, i < info.attack_param.size() ? info.attack_param[i].damage : 0, "PlayerNormalAttack", GSvector3::zero()
                );
            });
        }
        // 対象のモーションの時に生成するエフェクト
        set_effect_event(motion, info.attack_effect_event[i], true);
    }

    // スキルのアニメーションイベントを追加
    set_generate_collider(Motion::Skill, "PlayerNormalSkill", info.skill_event, GSvector3{ 0.0f, 0.0f, 0.0f });
    set_effect_event(Motion::Skill, info.skill_effect_event);
    
    // 回避攻撃のアニメーションイベントを追加
    set_generate_collider(Motion::AvoidAttack, "PlayerAvoidAttack", info.avoid_attack_event, GSvector3{ 1.0f, 0.0f, 1.0f } * 0.2f);
    set_effect_event(Motion::AvoidAttack, info.avoid_attack_effect_event);

    // 回避成功攻撃のアニメーションイベントを追加
    set_generate_collider(Motion::AvoidSuccessAttack, "PlayerAvoidSuccessAttack", info.avoid_success_attack_event, GSvector3{ 1.0f, 0.0f, 1.0f } * 0.3f);
    set_effect_event(Motion::AvoidSuccessAttack, info.avoid_success_attack_effect_event);

    // 回避成功スキルのアニメーションイベントを追加
    set_generate_collider(Motion::AvoidSuccessSkill, "PlayerSuccessSkill", info.avoid_success_skill_event, GSvector3{ 0.0f, 0.0f, 0.0f });
    set_effect_event(Motion::AvoidSuccessSkill, info.avoid_success_skill_effect_event);
}

void Player::generate_attack_collider(const GSvector3& offset, float radius, int damage, const std::string& name, const GSvector3& external_velocity) {
    GSmatrix4 m = local_to_world(offset, GSvector3::zero(), GSvector3::one());
    GSvector3 ev = transform_.forward() * external_velocity;

    WeaponData::Data weapon = world_->game_save_data().inventory().weapon();
    int dmg = (weapon.is_empty() ? 0 : weapon.damage) + damage;

    world_->generate_attack_collider(radius, m.position(), this, dmg, name, 0.1f, 0.0f, ev.sqrMagnitude() > 0.01f ? ev : GSvector3::zero());
}

bool Player::is_root_motion_state() const {
    if (MyLib::is_in(get_current_motion(),
        (GSuint)Motion::AvoidAttack,
        (GSuint)Motion::AvoidSuccessAttack
    )) return true;
    if (MyLib::is_in(state_.get_current_state(),
        (GSuint)PlayerStateType::Attack
    )) return true;
    return false;
}

void Player::on_air() {
    if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)PlayerStateType::Idle,
        (GSuint)PlayerStateType::Avoid,
        (GSuint)PlayerStateType::Hurt,
        (GSuint)PlayerStateType::Dead,
        (GSuint)PlayerStateType::Jump,
        (GSuint)PlayerStateType::Fall,
        (GSuint)PlayerStateType::Skill
	)) return;

	change_state((GSuint)PlayerStateType::Fall, Motion::Fall, true);
}

void Player::on_ground() {
	if (is_grounded_) return;
	if (MyLib::is_in(
		state_.get_current_state(),
		(GSuint)PlayerStateType::Idle,
        (GSuint)PlayerStateType::Avoid,
		(GSuint)PlayerStateType::Hurt,
		(GSuint)PlayerStateType::Dead,
		(GSuint)PlayerStateType::Land,
        (GSuint)PlayerStateType::Move,
        (GSuint)PlayerStateType::Skill
	)) return;

	change_state((GSuint)PlayerStateType::Land, Motion::Land, false);
    int handle = play_effect((GSuint)EffectID::OnGroundSmoke, GSvector3::zero());
    const GScolor color{ 1.0f, 1.0f, 1.0f, 0.25f };
    gsSetEffectColor(handle, &color);

    SE::play((GSuint)SEID::Land);
}

