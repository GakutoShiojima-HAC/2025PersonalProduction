#include "Elemental.h"
#include "Assets.h"
#include "Engine/Utils/Check.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/MyMath.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Sound/SE.h"

#include "Actor/Magic/FireBall/FireBall.h"
#include "Actor/Magic/ExplodeRoad/ExplodeRoad.h"

#include "State/Elemental/ElementalState.h"
#include "State/Actor/Character/CharacterMotionEndToAnyState.h"
#include "State/Actor/Character/CharacterMotionEndToDieState.h"
#include "State/Elemental/ElementalAttack1State.h"
#include "State/Elemental/ElementalIdleState.h"
#include "State/Elemental//ElementalMoveState.h"

// 無敵時間(秒)
const float INVINCIBLE_TIME{ 0.25f };

Elemental::Elemental(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info, const ElementalInfo& info) :
    MyEnemy{ world, position, rotate, my_info },
    info_{ info } {
    add_state();

    use_force_external_ = false;

    set_motion_attack_event(info_.motion_attack1, my_info_.attack_data[info_.motion_attack1]);
    set_motion_attack_event(info_.motion_attack2, my_info_.attack_data[info_.motion_attack2]);
    mesh_.add_animation_event(info_.motion_spell, my_info_.attack_data[info_.motion_spell].attack_time, [=] { generate_spell_attack(); });
    change_state_and_motion((GSuint)ElementalStateType::Idle);
    save_current_state();
}

void Elemental::draw_gui() const {
    draw_boss_bar();
}

void Elemental::take_damage(Actor& other, const int damage) {
    if (MyLib::is_in(state_.get_current_state(),
        (GSuint)ElementalStateType::Dead
    )) return;
    if (invincible_timer() > 0.0f) return;

    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
    invincible_timer_ = INVINCIBLE_TIME;

    if (hp_ <= 0) {
        change_state_and_motion((GSuint)ElementalStateType::Dead);
    }
    else {
        // 怯む確率
        const bool falter = world_->is_avoid_effect() ? true : MyRandom::random_float(0.0f, 1.0f) <= my_info_.falter_rate;
        if (target_ == nullptr || falter) {
            // 一度Idleにしてモーションをリセット
            mesh_.change_motion(info_.motion_idle, true);
            change_state_and_motion((GSuint)ElementalStateType::Hurt);
        }
    }

    // 攻撃対象をターゲットにする
    Character* target = dynamic_cast<Character*>(&other);
    if (target != nullptr && !target->is_dead_state()) {
        target_ = target;
    }
}

bool Elemental::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)ElementalStateType::Dead) || is_dead_;
}

void Elemental::change_state_and_motion(const GSuint state_num) {
    bool loop{ true };
    GSuint motion{ info_.motion_idle };

    switch ((ElementalStateType)state_num) {
    case ElementalStateType::Idle:      loop = true;    motion = info_.motion_idle;         break;
    case ElementalStateType::Angry:     loop = false;   motion = info_.motion_angry;        break;
    case ElementalStateType::MoveF:     loop = true;    motion = info_.motion_move;         break;
    case ElementalStateType::Hurt:      loop = false;   motion = info_.motion_hurt;         break;
    case ElementalStateType::Dead:      loop = false;   motion = info_.motion_dead;         break;
    case ElementalStateType::Attack1:   loop = false;   motion = info_.motion_attack1;      break;
    case ElementalStateType::Attack2:   loop = false;   motion = info_.motion_attack2;      break;
    case ElementalStateType::Spell:     loop = false;   motion = info_.motion_spell;        break;
    default:                            loop = true;    motion = info_.motion_idle;         break;
    }

    change_state(state_num, motion, loop);
}

void Elemental::add_state() {
    state_.add_state((GSuint)ElementalStateType::Idle, make_shared<ElementalIdleState>(*this));
    state_.add_state((GSuint)ElementalStateType::Angry, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)ElementalStateType::MoveF, info_.motion_move, true));
    state_.add_state((GSuint)ElementalStateType::MoveF, make_shared<ElementalMoveState>(*this));
    state_.add_state((GSuint)ElementalStateType::Hurt, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)ElementalStateType::Idle, info_.motion_idle, true));
    state_.add_state((GSuint)ElementalStateType::Dead, make_shared<CharacterMotionEndToDieState>(*this));
    state_.add_state((GSuint)ElementalStateType::Attack1, make_shared<ElementalAttack1State>(*this));
    state_.add_state((GSuint)ElementalStateType::Attack2, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)ElementalStateType::Idle, info_.motion_idle, true));
    state_.add_state((GSuint)ElementalStateType::Spell, make_shared<CharacterMotionEndToAnyState>(*this, (GSuint)ElementalStateType::Idle, info_.motion_idle, true));
}

bool Elemental::is_root_motion_state() const {
    return false;
}

const ElementalInfo& Elemental::info() const {
    return info_;
}

void Elemental::generate_spell_attack() {
    // 召喚座標
    GSvector3 position = local_to_world(my_info_.attack_data[info_.motion_spell].offset, GSvector3::zero(), GSvector3::one()).position();
    GSvector3 velocity;
    GSvector3 to;

    // ダメージ
    const int damage = my_info_.damage + my_info_.attack_data[info_.motion_spell].damage;

    switch (info_.spell_type) {
    case ElementalSpellType::FireBall:
        velocity = (target_ == nullptr ? transform_.forward() : target_->collider().center - position).normalized() * 15.0f / cFPS;
        world_->add_actor(new FireBall{ world_, position, velocity, 5.0f, this, damage });
        break;

    case ElementalSpellType::ExplodeRoad:
        to = target_ == nullptr ? transform_.forward() : target_->collider().center;
        to.y = position.y;
        velocity = (to - position).normalized() * 2.1f;
        world_->add_actor(new ExplodeRoad{ world_, position, velocity, this, damage, 5, 0.25f });
        break;

    default:
        break;
    }
}
