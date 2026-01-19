#include "Viking.h"
#include "Assets.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Sound/SE.h"

#include "State/Viking/VikingState.h"
#include "State/Actor/Character/CharacterMotionEndToAnyState.h"
#include "State/Actor/Character/CharacterMotionEndToDieState.h"
#include "State/Viking/VikingIdleState.h"
#include "State/Viking/VikingMoveState.h"
#include "State/Viking/VikingBlockState.h"
#include "State/Viking/VikingRushState.h"

#include "State/Actor/MyEnemy/MyEnemyLeapAttackState.h"
#include "State/Actor/MyEnemy/MyEnemyRetreatState.h"
#include "State/Actor/Character/CharacterLandingToAnyState.h"

#include <imgui/imgui.h>

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.35f };

Viking::Viking(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info) :
    MyEnemy{ world, position, rotate, my_info } {
    add_state();

    use_force_external_ = false;

    set_motion_attack_event(Motion::Attack, my_info_.attack_data[Motion::Attack]);
    mesh_.add_animation_event(Motion::JumpEnd, 3.0f, [=] { generate_land_attack(); });
    mesh_.add_animation_event(Motion::RushEnd, 33.0f, [=] { generate_land_attack(); });

    change_state_and_motion((GSuint)VikingStateType::Idle);
    save_current_state();
}

void Viking::draw_gui() const {
    draw_boss_bar();

    ImGui::Begin("Viking");
    ImGui::Text("state: %d, motion: %d", state_.get_current_state(), motion_);
    ImGui::End();
}

void Viking::take_damage(Actor& other, const int damage) {
    const GSuint current_state_num = state_.get_current_state();

    if (MyLib::is_in(current_state_num,
        (GSuint)VikingStateType::Dead,
        (GSuint)VikingStateType::BlockHit
    )) return;
    if (invincible_timer() > 0.0f) return;

    // 防御中ならカウンター
    if (current_state_num == (GSuint)VikingStateType::Block) {
        invincible_timer_ = INVINCIBLE_TIME;
        change_state_and_motion((GSuint)VikingStateType::BlockHit);
        return;
    }

    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
    invincible_timer_ = INVINCIBLE_TIME;

    if (hp_ <= 0) {
        change_state_and_motion((GSuint)VikingStateType::Dead);
    }
    else {
        // 怯む確率
        const bool falter = world_->is_avoid_effect() ? true : MyRandom::random_float(0.0f, 1.0f) <= my_info_.falter_rate;
        if (target_ == nullptr || falter) {
            // 一度Idleにしてモーションをリセット
            mesh_.change_motion(Motion::Idle, true);
            change_state_and_motion((GSuint)VikingStateType::Hurt);
        }
    }

    // 攻撃対象をターゲットにする
    Character* target = dynamic_cast<Character*>(&other);
    if (target != nullptr && !target->is_dead_state()) {
        target_ = target;
    }
}

bool Viking::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)VikingStateType::Dead) || is_dead_;
}

void Viking::change_state_and_motion(const GSuint state_num) {
    bool loop{ true };
    GSuint motion{ Motion::Idle };

    switch ((VikingStateType)state_num) {
    case VikingStateType::Idle:     loop = true;    motion = Motion::Idle;      break;
    case VikingStateType::Move:     loop = true;    motion = Motion::MoveF;     break;
    case VikingStateType::Retreat:  loop = true;    motion = Motion::MoveB;     break;
    case VikingStateType::Angry:    loop = false;   motion = Motion::Angry;     break;
    case VikingStateType::Hurt:     loop = false;   motion = Motion::Hurt;      break;
    case VikingStateType::Dead:     loop = false;   motion = Motion::Dead;      break;
    case VikingStateType::Block:    loop = true;    motion = Motion::Block;     break;
    case VikingStateType::BlockHit: loop = false;   motion = Motion::BlockHit;  break;
    case VikingStateType::Attack:   loop = false;   motion = Motion::Attack;    break;
    case VikingStateType::JumpStart:    loop = false;   motion = Motion::JumpStart;     break;
    case VikingStateType::RushCharge:   loop = false;   motion = Motion::RushCharge;    break;
    case VikingStateType::RushEnd:  loop = false;   motion = Motion::RushEnd;   break;
    default:                        loop = true;    motion = Motion::Idle;      break;
    }

    change_state(state_num, motion, loop);
}

void Viking::add_state() {
    state_.add_state((GSuint)VikingStateType::Idle, make_shared<VikingIdleState>(*this));
    state_.add_state((GSuint)VikingStateType::Angry, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::Move, Motion::MoveF, true));
    state_.add_state((GSuint)VikingStateType::Move, make_shared<VikingMoveState>(*this));
    state_.add_state((GSuint)VikingStateType::Hurt, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::Idle, Motion::Idle, true));
    state_.add_state((GSuint)VikingStateType::Dead, make_shared<CharacterMotionEndToDieState>(*this));
    state_.add_state((GSuint)VikingStateType::Block, make_shared<VikingBlockState>(*this));
    state_.add_state((GSuint)VikingStateType::BlockHit, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::Attack, Motion::Attack, false));
    state_.add_state((GSuint)VikingStateType::Attack, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::Retreat, Motion::MoveB, true));
    state_.add_state((GSuint)VikingStateType::JumpStart, make_shared<MyEnemyLeapAttackState>(*this, (GSuint)VikingStateType::Jumping, Motion::Jumping, true, info_.leap_power));
    state_.add_state((GSuint)VikingStateType::Jumping, make_shared<CharacterLandingToAnyState>(*this, (GSuint)VikingStateType::JumpEnd, Motion::JumpEnd, false));
    state_.add_state((GSuint)VikingStateType::JumpEnd, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::Idle, Motion::Idle, true));
    state_.add_state((GSuint)VikingStateType::RushCharge, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::RushStart, Motion::RushStart, false));
    state_.add_state((GSuint)VikingStateType::RushStart, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::RushMove, Motion::Rushing, true));
    state_.add_state((GSuint)VikingStateType::RushMove, make_shared<VikingRushState>(*this));
    state_.add_state((GSuint)VikingStateType::RushEnd, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)VikingStateType::Block, Motion::Block, true));
    const float d = my_info_.attack_data.find(VikingMotion::Attack)->second.detection_radius;
    state_.add_state((GSuint)VikingStateType::Retreat, make_shared<MyEnemyRetreatState>(*this, (GSuint)VikingStateType::Idle, Motion::Idle, true, d));
}

bool Viking::is_root_motion_state() const {
    const GSuint current_state_num = state_.get_current_state();

    if (MyLib::is_in(current_state_num,
        (GSuint)VikingStateType::RushEnd
    )) return true;

    return false;
}

const VikingInfo& Viking::info() const {
    return info_;
}

void Viking::generate_land_attack() {
    const GSvector3 pos = transform_.position();
    const GSvector3 v = MyRandom::random_vec3(GSvector3{ -0.075f, 0.15f, -0.075f }, GSvector3{ 0.075f, 0.2f, 0.075f });
    world_->generate_attack_collider(2.0f, pos, this, info_.landing_damage, "VikingLandingAttack", 0.1f, 0.0f, v);
    int handle = play_effect((GSuint)EffectID::DustLarge, GSvector3::zero());
    const GScolor color{ 1.0f, 1.0f, 1.0f, 0.25f };
    gsSetEffectColor(handle, &color);

    SE::play((GSuint)SEID::HardLand);
}

