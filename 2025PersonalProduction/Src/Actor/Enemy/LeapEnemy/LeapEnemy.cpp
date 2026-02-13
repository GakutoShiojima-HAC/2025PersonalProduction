#include "LeapEnemy.h"
#include "Engine/Utils/Check.h"
#include "Assets.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/MyMath.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Sound/SE.h"

#include "State/LeapEnemy/LeapEnemyState.h"
#include "State/Actor/MyEnemy/MyEnemyAttackState.h"
#include "State/Actor/Character/CharacterMotionEndToAnyState.h"
#include "State/Actor/Character/CharacterMotionEndToDieState.h"
#include "State/Actor/MyEnemy/MyEnemyLeapAttackState.h"
#include "State/Actor/Character/CharacterLandingToAnyState.h"
#include "State/LeapEnemy/LeapEnemyIdleState.h"
#include "State/LeapEnemy/LeapEnemyMoveState.h"

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.25f };

LeapEnemy::LeapEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info, const LeapEnemyInfo& info) :
    MyEnemy{ world, position, rotate, my_info },
    info_{ info } {
    add_state();

    set_motion_attack_event(info_.motion_attack, my_info_.attack_data[info_.motion_attack]);
    mesh_.add_animation_event(info_.motion_leapend, my_info_.attack_data[info_.motion_leapend].attack_time, [=] { generate_land_attack(); });
    change_state_and_motion((GSuint)LeapEnemyStateType::Idle);
    save_current_state();
}

void LeapEnemy::take_damage(Actor& other, const int damage) {
    if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)LeapEnemyStateType::Idle,
        (GSuint)LeapEnemyStateType::Dead
    )) return;
    if (invincible_timer() > 0.0f) return;

    // ダメージを受ける
    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
    invincible_timer_ = INVINCIBLE_TIME;

    // 攻撃対象をターゲットにする
    Character* target = dynamic_cast<Character*>(&other);
    if (target != nullptr && !target->is_dead_state()) {
        target_ = target;
    }

    // ステート番号を保存
    if (!MyLib::is_in(state_.get_current_state(),
        (GSuint)LeapEnemyStateType::Hurt,
        (GSuint)LeapEnemyStateType::Attack
    )) save_current_state();

    // 死亡
    if (hp_ <= 0) {
        change_state_and_motion((GSuint)LeapEnemyStateType::Dead);
    }
    // 生きている
    else {
        // 怯むか？
        if (falter_counter_ > 0 || falter_counter_ < 0) {
            // 一度Idleにしてモーションをリセット
            mesh_.change_motion(info_.motion_idle, true);
            change_state_and_motion((GSuint)LeapEnemyStateType::Hurt);
            falter_counter_--;  // 減らす
            return;
        }
        // 怯まない
        return;
    }
}

bool LeapEnemy::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)LeapEnemyStateType::Dead) || is_dead_;
}

void LeapEnemy::change_state_and_motion(const GSuint state_num) {
    bool loop{ true };
    GSuint motion{ info_.motion_idle };

    switch ((LeapEnemyStateType)state_num) {
    case LeapEnemyStateType::Idle:      loop = true;    motion = info_.motion_idle;         break;
    case LeapEnemyStateType::Move:      loop = true;    motion = info_.motion_move;         break;
    case LeapEnemyStateType::Hurt:      loop = false;   motion = info_.motion_hurt;         break;
    case LeapEnemyStateType::Dead:      loop = false;   motion = info_.motion_dead;         break;
    case LeapEnemyStateType::Attack:    loop = false;   motion = info_.motion_attack;       break;
    case LeapEnemyStateType::LeapStart: loop = false;   motion = info_.motion_leap_start;   break;
    default:                            loop = true;    motion = info_.motion_idle;         break;
    }

    change_state(state_num, motion, loop);
}

void LeapEnemy::add_state() {
    state_.add_state((GSuint)LeapEnemyStateType::Idle, make_shared<LeapEnemyIdleState>(*this));
    state_.add_state((GSuint)LeapEnemyStateType::Move, make_shared<LeapEnemyMoveState>(*this));
    state_.add_state((GSuint)LeapEnemyStateType::Hurt, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)LeapEnemyStateType::Idle, info_.motion_idle, true));
    state_.add_state((GSuint)LeapEnemyStateType::Dead, make_shared<CharacterMotionEndToDieState>(*this));
    state_.add_state((GSuint)LeapEnemyStateType::Attack, make_shared<MyEnemyAttackState>(*this, (GSuint)LeapEnemyStateType::Idle, info_.motion_idle, true));
    state_.add_state((GSuint)LeapEnemyStateType::LeapStart, make_shared<MyEnemyLeapAttackState>(*this, (GSuint)LeapEnemyStateType::Leaping, info_.motion_leaping, true, info_.leap_power));
    state_.add_state((GSuint)LeapEnemyStateType::Leaping, make_shared<CharacterLandingToAnyState>(*this, (GSuint)LeapEnemyStateType::LeapEnd, info_.motion_leapend, false));
    state_.add_state((GSuint)LeapEnemyStateType::LeapEnd, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)LeapEnemyStateType::Move, info_.motion_move, true));
}

bool LeapEnemy::is_root_motion_state() const {
    return false;
}

const LeapEnemyInfo& LeapEnemy::info() const {
    return info_;
}

void LeapEnemy::generate_land_attack() {
    const GSvector3 pos = transform_.position();
    world_->generate_attack_collider(2.0f, pos, this, my_info_.damage + info_.landing_damage, name_ + "LandingAttack", 0.1f, 0.0f);
    int handle = play_effect((GSuint)EffectID::OnGroundSmoke, GSvector3::zero());
    const GScolor color{ 1.0f, 1.0f, 1.0f, 0.25f };
    gsSetEffectColor(handle, &color);

    //SE::play((GSuint)SEID::Land);
}
