#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Utils/Line.h"
#include "GameConfig.h"
#include <GSeffect.h>

void Pawn::take_damage(Actor& other, const int damage) {
    hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
}

void Pawn::on_hit_attack(AttackCollider& collider) {

}

bool Pawn::is_dead_state() const {
    return false;
}

GSmatrix4 Pawn::critical_position() const {
    return transform_.localToWorldMatrix();
}

int& Pawn::hp() {
    return hp_;
}

int Pawn::max_hp() const
{
    return max_hp_;
}

float& Pawn::invincible_timer() {
    return invincible_timer_;
}

bool Pawn::is_motion_end() const {
    return mesh_.is_motion_end();
}

GSuint Pawn::current_motion() const {
    return motion_;
}

float Pawn::currnent_motion_time() const {
    return mesh_.current_motion_time();
}

bool Pawn::current_motion_loop() const {
    return motion_loop_;
}

float Pawn::current_motion_end_time() const {
    return mesh_.motion_end_time();
}

bool& Pawn::freeze_motion() {
    return is_motion_freeze_;
}

void Pawn::update_invincible(float delta_time) {
    // 無敵時間が無ければ終了
    if (invincible_timer_ <= 0.0f) return;

    // 無敵時間を減らす
    invincible_timer_ = CLAMP(invincible_timer_ - delta_time / world_->timescale() / cFPS, 0.0f, FLT_MAX);
}

void Pawn::update_display_hp(float delta_time) {
    // 描画と現在で差があるか
    if (std::fabs(display_hp_ - (float)hp_) > 0.01f) {
        // 線形補間で合わせる
        display_hp_ += ((float)hp_ - display_hp_) * 9.0f * delta_time / world_->timescale() / cFPS;
    }
    else {
        // 誤差対策
        display_hp_ = (float)hp_;
    }
}

void Pawn::init_parameter(PawnParameter::Type type) {
    height_ = PawnParameter::height(type);

    collider_ = BoundingSphere{ PawnParameter::radius(type), GSvector3{ 0.0f, height_ / 2.0f, 0.0f } };
}

void Pawn::update_mesh(float delta_time) {
    if (!is_motion_freeze_) {
        // メッシュのモーションを更新
        mesh_.update(delta_time);   
    }
    // ルートモーションを適用
    if (is_root_motion_state()) {
        mesh_.apply_root_motion(transform_);
        collide_field();
    }
    // ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}

