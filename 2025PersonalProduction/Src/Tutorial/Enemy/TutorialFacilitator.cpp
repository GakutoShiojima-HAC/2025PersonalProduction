#include "TutorialFacilitator.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include "Engine/Utils/Check.h"
#include "Actor/Player/Player.h"
#include "State/Player/PlayerState.h"

#include "State/Actor/Character/CharacterMotionEndToAnyState.h"
#include "State/Actor/Character/CharacterMotionEndToDieState.h"

#include "State/TutorialFacilitatorState.h"
#include "State/TutorialFacilitatorIdleState.h"
#include "State/TutorialFacilitatorAttackInputState.h"
#include "State/TutorialFacilitatorChaseState.h"
#include "State/TutorialFacilitatorAvoidInputChaseState.h"
#include "State/TutorialFacilitatorAvoidInputState.h"
#include "State/TutorialFacilitatorAvoidAttackInputState.h"
#include "State/TutorialFacilitatorMoveState.h"

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.125f };

TutorialFacilitator::TutorialFacilitator(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info) :
    MyEnemy{ world, position, rotate, my_info } {
	add_state();

    // 攻撃アニメーションイベントを登録
    if (is_attack_motion(Motion::Attack)) {
        set_motion_attack_event(Motion::Attack, my_info_.attack_data[Motion::Attack]);
    }

	change_state((GSuint)TutorialFacilitatorStateType::Idle, Motion::Idle, true);
    save_current_state();
}

void TutorialFacilitator::update(float delta_time) {
    is_navmesh_update_ = false; // リセット
    update_invincible(delta_time);
    update_state(delta_time);
    update_physics(delta_time);
    collide_field();
    update_mesh(delta_time);
}

void TutorialFacilitator::draw_gui() const {
    state_.draw_gui();
}

void TutorialFacilitator::take_damage(Actor& other, const int damage) {
	if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)TutorialFacilitatorStateType::Idle,
        (GSuint)TutorialFacilitatorStateType::Dead,
        (GSuint)TutorialFacilitatorStateType::AvoidInputChase,
        (GSuint)TutorialFacilitatorStateType::AvoidInput
    )) return;
    if (invincible_timer() > 0.0f) return;

    if (enable_damage_count_) {
        hp_ = CLAMP(hp_ - 1, 0, INT_MAX);   // 回数でカウントしたいのでダメージ量で減らさない
    }
    else {
        hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
    }
    invincible_timer_ = INVINCIBLE_TIME;

    // 回避を教える条件 残りHPが1になる
    if (enable_damage_count_ && hp_ <= 1) {
        change_state((GSuint)TutorialFacilitatorStateType::AvoidInputChase, Motion::Move, true);
        // ダメージを与える部分は学習終了
        enable_damage_count_ = false;
        return;
    }
    // 死亡
    if (hp_ <= 0) {
        change_state((GSuint)TutorialFacilitatorStateType::Dead, Motion::Dead, false);
        // TODO よくやったメッセージ？
    }
    else {
        // 学習中
        if (enable_damage_count_) {
            change_state((GSuint)TutorialFacilitatorStateType::AttackInputHurt, Motion::Hurt, false);
            return;
        }
        else {
            change_state((GSuint)TutorialFacilitatorStateType::Hurt, Motion::Hurt, false);
            return;
        }
    }
    return;
}

bool TutorialFacilitator::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)TutorialFacilitatorStateType::Dead) || is_dead_;
}

void TutorialFacilitator::change_state_and_motion(const GSuint state_num) {
    // 使わない
}

void TutorialFacilitator::init_tutorial() {
    hp_ = my_info().hp;

    is_avoid_execute_ = false;
    enable_damage_count_ = true;
    is_motion_freeze_ = false;
}

bool TutorialFacilitator::is_avoid_effect() const {
    return world_->is_avoid_effect();
}

bool& TutorialFacilitator::enable_draw_game_info_gui() {
    return world_->enable_draw_game_info_gui();
}

void TutorialFacilitator::pause_avoid_effect() {
    world_->pause_avoid_effect();
}

void TutorialFacilitator::resume_avoid_effect() {
    world_->resume_avoid_effect();
}

void TutorialFacilitator::add_state() {
	state_.add_state((GSuint)TutorialFacilitatorStateType::Idle, make_shared<TutorialFacilitatorIdleState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Dead, make_shared<CharacterMotionEndToDieState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Hurt, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)TutorialFacilitatorStateType::Move, Motion::Move, true));

    state_.add_state((GSuint)TutorialFacilitatorStateType::AttackInput, make_shared<TutorialFacilitatorAttackInputState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Chase, make_shared<TutorialFacilitatorChaseState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::AttackInputHurt, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)TutorialFacilitatorStateType::Chase, Motion::Move, true));

    state_.add_state((GSuint)TutorialFacilitatorStateType::AvoidInputChase, make_shared<TutorialFacilitatorAvoidInputChaseState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::AvoidInput, make_shared<TutorialFacilitatorAvoidInputState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::AvoidAttackInput, make_shared<TutorialFacilitatorAvoidAttackInputState>(*this));

    state_.add_state((GSuint)TutorialFacilitatorStateType::Attack, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)TutorialFacilitatorStateType::Move, Motion::Move, true));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Move, make_shared<TutorialFacilitatorMoveState>(*this));
}
