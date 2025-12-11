#include "TutorialFacilitator.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include "Engine/Utils/Check.h"
#include "State/Player/PlayerState.h"

#include "State/TutorialFacilitatorState.h"
#include "State/TutorialFacilitatorAttackInputState.h"
#include "State/TutorialFacilitatorAvoidAttackInputState.h"
#include "State/TutorialFacilitatorAvoidInputState.h"
#include "State/TutorialFacilitatorHurtState.h"
#include "State/TutorialFacilitatorDeadState.h"
#include "State/TutorialFacilitatorIdleState.h"
#include "State/TutorialFacilitatorAttackState.h"
#include "State/TutorialFacilitatorEventState.h"

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.25f };

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

void TutorialFacilitator::draw_gui() const {
    const GSuint current = state_.get_current_state();

    // 回避入力UIを描画
    if (target_ != nullptr && target_->current_state_num() == (GSuint)PlayerStateType::Avoid && is_avoid_effect()) {
          Canvas::draw_sprite_text(
              "攻撃チャンス！",
              GSvector2{ 0.0f, 625.0f },
              60,
              cFONT,
              GS_FONT_NORMAL,
              GScolor{ 1.0f, 0.76f, 0.219f, 1.0f },
              Anchor::TopCenter, Anchor::Center
          );
    }

    // イベント時の画像を描画
    if (current == (GSuint)TutorialFacilitatorStateType::Event) {
        Canvas::draw_texture(event_texture_id_, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });
    }
}

void TutorialFacilitator::take_damage(Actor& other, const int damage) {
	if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)TutorialFacilitatorStateType::Idle,
        (GSuint)TutorialFacilitatorStateType::Event,
        (GSuint)TutorialFacilitatorStateType::AvoidInput,
        (GSuint)TutorialFacilitatorStateType::Dead
    )) return;
    if (invincible_timer() > 0.0f) return;

    // 回避攻撃処理の時、回避空間から攻撃されないかつ、プレイヤーの状態が特殊攻撃処理でないとダメージを受けない
    const bool is_avoid_atack_input_state = MyLib::is_in(
        state_.get_current_state(),
        (GSuint)TutorialFacilitatorStateType::Attack,
        (GSuint)TutorialFacilitatorStateType::AvoidAttackInput
    );
    const bool is_player_skill_state = target_->current_state_num() == (GSuint)PlayerStateType::Skill; 
    if (is_avoid_atack_input_state && !is_avoid_effect()) return;
    if (is_avoid_atack_input_state && !is_player_skill_state) return;

	hp_ = CLAMP(hp_ - 1, 0, INT_MAX);   // 回数でカウントしたいのでダメージを使わない
	invincible_timer_ = INVINCIBLE_TIME;

    // ステート番号を保存
    if (!MyLib::is_in(state_.get_current_state(),
        (GSuint)TutorialFacilitatorStateType::Hurt
    )) save_current_state();

    // 死亡
	if (hp_ <= 0) {
        change_state((GSuint)TutorialFacilitatorStateType::Dead, Motion::Dead, false);
        // よくやったメッセージ？
    }
    // 攻撃入力を十分に教えたので回避入力を教える
    else if (hp_ <= 1) {
        prev_state_num_ = (GSuint)TutorialFacilitatorStateType::AvoidInput;
        prev_motion_num_ = Motion::Move;
        prev_motion_loop_ = true;
        on_avoid_input_event();
        change_state((GSuint)TutorialFacilitatorStateType::Event, Motion::Hurt, false);
    }
    // 攻撃入力を教えている
    else {
        change_state((GSuint)TutorialFacilitatorStateType::Hurt, Motion::Hurt, false);
    }
}

bool TutorialFacilitator::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)TutorialFacilitatorStateType::Dead) || is_dead_;
}

void TutorialFacilitator::change_state_and_motion(const GSuint state_num) {
    // 使わない
}

bool TutorialFacilitator::is_avoid_effect() const {
    return world_->is_avoid_effect();
}

void TutorialFacilitator::on_attack_input_event() {
    event_texture_id_ = (GSuint)TextureID::TutorialAttackInputTexture;
}

void TutorialFacilitator::on_avoid_input_event() {
    event_texture_id_ = (GSuint)TextureID::TutorialAvoidInputTexture;
}

bool& TutorialFacilitator::enable_game_info_gui() {
    return world_->enable_draw_game_info_gui();
}

void TutorialFacilitator::add_state() {
	state_.add_state((GSuint)TutorialFacilitatorStateType::Idle, make_shared<TutorialFacilitatorIdleState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Dead, make_shared<TutorialFacilitatorDeadState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Hurt, make_shared<TutorialFacilitatorHurtState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Event, make_shared<TutorialFacilitatorEventState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::Attack, make_shared<TutorialFacilitatorAttackState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::AttackInput, make_shared<TutorialFacilitatorAttackInputState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::AvoidInput, make_shared<TutorialFacilitatorAvoidInputState>(*this));
    state_.add_state((GSuint)TutorialFacilitatorStateType::AvoidAttackInput, make_shared<TutorialFacilitatorAvoidAttackInputState>(*this));
}
