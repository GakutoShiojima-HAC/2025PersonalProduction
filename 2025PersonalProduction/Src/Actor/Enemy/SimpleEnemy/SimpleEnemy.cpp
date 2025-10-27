#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Utils/Check.h"
#include "Assets.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/MyMath.h"

#include "State/SimpleEnemy/SimpleEnemyAttackState.h"
#include "State/SimpleEnemy/SimpleEnemyDeadState.h"
#include "State/SimpleEnemy/SimpleEnemyFindState.h"
#include "State/SimpleEnemy/SimpleEnemyHurtState.h"
#include "State/SimpleEnemy/SimpleEnemyIdleState.h"
#include "State/SimpleEnemy/SimpleEnemyMoveState.h"
#include "State/SimpleEnemy/SimpleEnemySearchState.h"

SimpleEnemy::SimpleEnemy(IWorld* world, const GSvector3& position, const GSvector3& lookat, const SimpleEnemyInfo& info) :
    info_{ info } {
    world_ = world;
    tag_ = ActorTag::Enemy;
    name_ = info.name;
    hp_ = info.hp;
    height_ = info.height;
    head_offset_ = height_;
    foot_offset_ = info.foot_offset;

    mesh_ = { info.skinmesh, info.skinmesh, info.skinmesh };
    add_state();

    // 衝突判定球を生成
    collider_ = BoundingSphere{ info.radius, GSvector3{ 0.0f, height_ / 2.0f, 0.0f } };

    // 攻撃アニメーションイベントを生成
    mesh_.add_animation_event(info_.motion_attack, info_.attack_event_time, [=] { generate_attack_collider(); });

    transform_.position(position);
    transform_.lookAt(lookat);
    mesh_.transform(transform_.localToWorldMatrix());
    collide_field();
    change_state((GSuint)SimpleEnemyStateType::Search, info.motion_idle, true);
}

void SimpleEnemy::update(float delta_time) {
    update_invincible(delta_time);
    update_state(delta_time);
    update_gravity(delta_time);
    update_mesh(delta_time);
}

void SimpleEnemy::draw() const {
    mesh_.draw();
}

void SimpleEnemy::draw_gui() const {
    // TODO HP?
}

void SimpleEnemy::take_damage(Actor& other, const int damage) {
    if (MyLib::is_in(
        state_.get_current_state(),
        (GSuint)SimpleEnemyStateType::Idle,
        (GSuint)SimpleEnemyStateType::Dead
    )) return;
    if (invincible_timer() > 0.0f) return;

    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);

    // ステート番号を保存
    if (!MyLib::is_in(state_.get_current_state(),
        (GSuint)SimpleEnemyStateType::Hurt,
        (GSuint)SimpleEnemyStateType::Attack
    )) prev_state_num_ = state_.get_current_state();

    if (hp_ <= 0) {
        change_state((GSuint)SimpleEnemyStateType::Dead, info_.motion_dead, false);
    }
    else {
        // 一度Idleにしてモーションをリセット
        mesh_.change_motion(info_.motion_idle, true);
        change_state((GSuint)SimpleEnemyStateType::Hurt, info_.motion_hurt, false);
    }
}

bool SimpleEnemy::is_dead_state() const {
    return MyLib::is_in(state_.get_current_state(), (GSuint)SimpleEnemyStateType::Dead) || is_dead_;
}

void SimpleEnemy::react(Actor& other) {
    if (MyLib::is_in(other.tag(), ActorTag::Enemy, ActorTag::Player)) collide_actor(other);
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

void SimpleEnemy::update_mesh(float delta_time) {
    // メッシュのモーションを更新
    mesh_.update(delta_time);
    // ルートモーションを適用
    if (is_root_motion_state()) mesh_.apply_root_motion(transform_);
    // ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}

const SimpleEnemyInfo& SimpleEnemy::get_info() const {
    return info_;
}

void SimpleEnemy::save_prev_state() {
    prev_state_num_ = state_.get_current_state();
}

void SimpleEnemy::change_prev_state() {
    bool loop{ true };
    GSuint motion = get_motion((SimpleEnemyStateType)prev_state_num_, &loop);

    change_state(prev_state_num_, motion, loop);
}

GSuint SimpleEnemy::prev_state_num() const {
    return prev_state_num_;
}

GSuint SimpleEnemy::get_motion(SimpleEnemyStateType state, bool* loop) const {
    switch (state) {
    case SimpleEnemyStateType::Idle: if (loop != nullptr) *loop = true; return info_.motion_idle;
    case SimpleEnemyStateType::Attack: if (loop != nullptr) *loop = false; return info_.motion_attack;
    case SimpleEnemyStateType::Dead: if (loop != nullptr) *loop = false; return info_.motion_dead;
    case SimpleEnemyStateType::Find: if (loop != nullptr) *loop = true; return info_.motion_idle;
    case SimpleEnemyStateType::Hurt: if (loop != nullptr) *loop = false; return info_.motion_hurt;
    case SimpleEnemyStateType::Move: if (loop != nullptr) *loop = true; return info_.motion_move;
    case SimpleEnemyStateType::Search: if (loop != nullptr) *loop = true; return info_.motion_idle;
    default: if (loop != nullptr) *loop = true; return info_.motion_idle;
    }
}

bool SimpleEnemy::search_target() {
    if (target_ != nullptr) {
        // 死亡判定なら再検索
        if (target_->is_dead_state()) target_ = nullptr;
        // 存在しているなら真
        else return true;
    }
    std::vector<Character*> character = world_->find_character_with_tag(ActorTag::Player);
    if (character.empty()) {
        target_ = nullptr;
        return false;
    }
    // 一番近いキャラクターを探す
    float min_length{ FLT_MAX };
    Character* target{ nullptr };
    const float fov = info_.search_fov;
    const float len = info_.search_length;
    for (const auto& chara : character) {
        // 死亡していたらスキップ
        if (chara->is_dead_state()) continue;
        // 自分の視野の外なら見えていないとしてスキップ
        if (MyMath::to_target_angle(transform_.position(), transform_.forward(), chara->transform().position()) > fov) continue;
        // 障害物があったら見えていないとしてスキップ
        Line line{ collider().center, chara->collider().center };
        if (world_->get_field()->collide(line)) continue;
        // 索敵範囲の外なら見えていないとしてスキップ
        float length = (line.end - line.start).magnitude();
        if (length > len) continue;
        // 一番近ければ対象を更新
        if (length < min_length) {
            min_length = length;
            target = chara;
        }
    }

    target_ = target;
    return target_ != nullptr;
}

Character* SimpleEnemy::target() {
    return target_;
}

void SimpleEnemy::move(const GSvector3& velocity, GSvector3* foward, float trun_angle) {
    non_penetrating_move(velocity, foward, trun_angle);
}

bool SimpleEnemy::is_root_motion_state() const {
    return false;
}

void SimpleEnemy::generate_attack_collider() {
    GSmatrix4 m = local_to_world(info_.attack_offset, GSvector3::zero(), GSvector3::one());
    world_->generate_attack_collider(info_.attack_radius, m.position(), this, info_.attack_damage, "SimpleEnemyAttack", 0.1f, 0.0f);
}
