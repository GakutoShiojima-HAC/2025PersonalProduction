#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Utils/Check.h"
#include "Assets.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/MyMath.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Sound/SE.h"

#include "State/SimpleEnemy/SimpleEnemyAttackState.h"
#include "State/SimpleEnemy/SimpleEnemyDeadState.h"
#include "State/SimpleEnemy/SimpleEnemyFindState.h"
#include "State/SimpleEnemy/SimpleEnemyHurtState.h"
#include "State/SimpleEnemy/SimpleEnemyIdleState.h"
#include "State/SimpleEnemy/SimpleEnemyMoveState.h"
#include "State/SimpleEnemy/SimpleEnemySearchState.h"

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.25f };

SimpleEnemy::SimpleEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info, const SimpleEnemyInfo& info) :
    MyEnemy{ world, position, rotate, my_info },
    info_{ info } {
    add_state();

    // 攻撃アニメーションイベントを登録
    if (is_attack_motion(info.motion_attack)) {
        set_motion_attack_event(info.motion_attack, my_info_.attack_data[info.motion_attack]);
    }

    change_state_and_motion((GSuint)SimpleEnemyStateType::Search);
    save_current_state();
}

void SimpleEnemy::take_damage(Actor& other, const int damage) {
    if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)SimpleEnemyStateType::Idle,
        (GSuint)SimpleEnemyStateType::Dead
    )) return;
    if (invincible_timer() > 0.0f) return;

    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
    invincible_timer_ = INVINCIBLE_TIME;

    // ステート番号を保存
    if (!MyLib::is_in(state_.get_current_state(),
        (GSuint)SimpleEnemyStateType::Hurt,
        (GSuint)SimpleEnemyStateType::Attack
    )) save_current_state();

    if (hp_ <= 0) {
        change_state_and_motion((GSuint)SimpleEnemyStateType::Dead);
    }
    else {
        // 怯む確率
        if (target_ == nullptr || MyRandom::random_float(0.0f, 1.0f) <= my_info_.falter_rate) {
            // 一度Idleにしてモーションをリセット
            mesh_.change_motion(info_.motion_idle, true);
            change_state_and_motion((GSuint)SimpleEnemyStateType::Hurt);
        }
    }

    // 攻撃対象をターゲットにする
    Character* target = dynamic_cast<Character*>(&other);
    if (target != nullptr && !target->is_dead_state()) {
        target_ = target;
    }
}

bool SimpleEnemy::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)SimpleEnemyStateType::Dead) || is_dead_;
}

void SimpleEnemy::change_state_and_motion(const GSuint state_num) {
    bool loop{ true };
    GSuint motion{ info_.motion_idle };

    switch (SimpleEnemyStateType(state_num)) {
    case SimpleEnemyStateType::Idle:    loop = true;    motion = info_.motion_idle;     break;
    case SimpleEnemyStateType::Attack:  loop = false;   motion = info_.motion_attack;   break;
    case SimpleEnemyStateType::Dead:    loop = false;   motion = info_.motion_dead;     break;
    case SimpleEnemyStateType::Find:    loop = true;    motion = info_.motion_idle;     break;
    case SimpleEnemyStateType::Hurt:    loop = false;   motion = info_.motion_hurt;     break;
    case SimpleEnemyStateType::Move:    loop = true;    motion = info_.motion_move;     break;
    case SimpleEnemyStateType::Search:  loop = true;    motion = info_.motion_idle;     break;
    default:                            loop = true;    motion = info_.motion_idle;     break;
    }

    change_state(state_num, motion, loop);
}

void SimpleEnemy::add_state() {
    state_.add_state((GSuint)SimpleEnemyStateType::Attack, make_shared<SimpleEnemyAttackState>(*this));
    state_.add_state((GSuint)SimpleEnemyStateType::Dead, make_shared<SimpleEnemyDeadState>(*this));
    state_.add_state((GSuint)SimpleEnemyStateType::Find, make_shared<SimpleEnemyFindState>(*this));
    state_.add_state((GSuint)SimpleEnemyStateType::Hurt, make_shared<SimpleEnemyHurtState>(*this));
    state_.add_state((GSuint)SimpleEnemyStateType::Idle, make_shared<SimpleEnemyIdleState>(*this));
    state_.add_state((GSuint)SimpleEnemyStateType::Move, make_shared<SimpleEnemyMoveState>(*this));
    state_.add_state((GSuint)SimpleEnemyStateType::Search, make_shared<SimpleEnemySearchState>(*this));
}

const SimpleEnemyInfo& SimpleEnemy::info() const {
    return info_;
}
