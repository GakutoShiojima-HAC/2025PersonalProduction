#include "Lich.h"
#include "Assets.h"
#include "Engine/Utils/Check.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/MyMath.h"
#include "Engine/Utils/MyRandom.h"

#include "State/Lich/LichState.h"
#include "State/Lich/LichIdleState.h"
#include "State/Lich/LichHurtState.h"
#include "State/Lich/LichDeadState.h"
#include "State/Lich/LichAttackState.h"
#include "State/Lich/LichSpellState.h"

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.5f };

Lich::Lich(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info, const LichInfo& info) :
    MyEnemy{ world, position, rotate, my_info },
    info_{ info } {
    add_state();

    // 攻撃アニメーションイベントを生成
    mesh_.add_animation_event(Motion::Attack1, 35.0f, [=] { generate_attack_collider(); });
    mesh_.add_animation_event(Motion::Attack2, 44.0f, [=] { generate_attack_collider(); });
    mesh_.add_animation_event(Motion::Spell1, 36.0f, [=] { generate_spell_a(); });
    mesh_.add_animation_event(Motion::Spell2, 23.0f, [=] { generate_spell_b(); });

    change_state_and_motion((GSuint)LichStateType::Idle);
    save_current_state();
}

void Lich::take_damage(Actor& other, const int damage) {
    if (MyLib::is_in(state_.get_current_state(),
        (GSuint)LichStateType::Dead
    )) return;
    if (invincible_timer() > 0.0f) return;

    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
    invincible_timer_ = INVINCIBLE_TIME;

    if (hp_ <= 0) {
        change_state_and_motion((GSuint)LichStateType::Dead);
    }
    else {
        // 攻撃中なら確定で怯む
        bool is_attacking = MyLib::is_in(state_.get_current_state(), (GSuint)LichStateType::Attack, (GSuint)LichStateType::Spell);
        // 確率
        bool is_falter = target_ == nullptr || MyRandom::random_float(0.0f, 1.0f) <= my_info_.falter_rate;

        if (is_attacking || is_falter) {
            // 一度Idleにしてモーションをリセット
            mesh_.change_motion(Motion::Idle, true);
            change_state_and_motion((GSuint)LichStateType::Hurt);
        }
    }

    // 攻撃対象をターゲットにする
    Character* target = dynamic_cast<Character*>(&other);
    if (target != nullptr && !target->is_dead_state()) {
        target_ = target;
    }
}

bool Lich::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)LichStateType::Dead) || is_dead_;
}

void Lich::change_state_and_motion(const GSuint state_num) {
    bool loop{ true };
    GSuint motion{ Motion::Idle };

    switch ((LichStateType)state_num) {
    case LichStateType::Idle:   loop = true;    motion = Motion::Idle;      break;
    case LichStateType::Hurt:   loop = false;   motion = Motion::hurt;      break;
    case LichStateType::Dead:   loop = false;   motion = Motion::Dead;      break;
    default:                    loop = true;    motion = Motion::Idle;      break;
    }

    change_state(state_num, motion, loop);
}

void Lich::add_state() {
    state_.add_state((GSuint)LichStateType::Idle, make_shared<LichIdleState>(*this));
    state_.add_state((GSuint)LichStateType::Hurt, make_shared<LichHurtState>(*this));
    state_.add_state((GSuint)LichStateType::Dead, make_shared<LichDeadState>(*this));
    state_.add_state((GSuint)LichStateType::Attack, make_shared<LichAttackState>(*this));
    state_.add_state((GSuint)LichStateType::Spell, make_shared<LichSpellState>(*this));
}

const LichInfo& Lich::info() const {
    return info_;
}

bool Lich::is_root_motion_state() const {
    return false;
}

void Lich::generate_spell_a() {
    // 投擲
}

void Lich::generate_spell_b() {
    // 地面から何か

    // HPで分岐する？
}
