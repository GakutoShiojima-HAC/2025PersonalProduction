#include "FireBall.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include <GSeffect.h>
#include "GameConfig.h"
#include "Engine/Utils/Line.h"

constexpr float RADIUS{ 0.4f };

FireBall::FireBall(IWorld* world, const GSvector3& position, const GSvector3& velocity, float life_time, Actor* owner, int damage) {
	world_ = world;
	tag_ = ActorTag::None;
    name_ = "FireBall";

    if (owner != nullptr && owner->tag() == ActorTag::Enemy) target_tag_ = ActorTag::Player;
    else target_tag_ = ActorTag::Enemy;

    owner_ = owner;
    life_time_ = life_time;
    timer_ = 0.0f;
    damage_ = damage;

    velocity_ = velocity;
	transform_.position(position);
    transform_.rotation(GSquaternion::lookRotation(velocity));

	enable_collider_ = true;
    collider_ = BoundingSphere{ RADIUS };

    effect_handle_ = play_effect((GSuint)EffectID::FireBall, transform_.position());
}

void FireBall::update(float delta_time) {
    // 寿命処理
    timer_ += delta_time / cFPS;
    if (timer_ > life_time_) {
        gsStopEffect(effect_handle_);
        is_dead_ = true;
        return;
    }

    // エフェクトに自身のワールド変換行列を設定
    GSmatrix4 world = transform_.localToWorldMatrix();
    gsSetEffectMatrix(effect_handle_, &world);

    // 寿命によって、だんだん透明にしていく（アルファ値を変化させる）
    GScolor start_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
    GScolor color = GScolor::lerp(end_color, start_color, 1.0f - timer_ / life_time_);
    gsSetEffectColor(effect_handle_, &color); // カラーを設定

    //// 衝突判定
    Line line;
    // 移動元
    line.start = transform_.position();
    // 移動先
    line.end = transform_.position() + velocity_ * delta_time;
    // 修正先座標
    GSvector3 intersect;
    if (world_->get_field()->collide(line, &intersect)) {
    	// 座標修正(フィールドと衝突)
    	transform_.position(intersect);
    	// 衝突したので削除
    	gsStopEffect(effect_handle_);
        hit();
    	return;
    }
    // 移動
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void FireBall::late_update(float delta_time) {
    if (owner_ != nullptr && owner_->is_dead()) {
        owner_ = nullptr;
    }
}

void FireBall::die() {
    // 外部から削除させない
}

void FireBall::react(Actor& other) {
    if (other.tag() == target_tag_) {
        hit();
        return;
    }
}

void FireBall::hit() {
    is_dead_ = true;
    gsStopEffect(effect_handle_);
    world_->generate_attack_collider(RADIUS * 2.0f, transform_.position(), owner_, damage_, "Attack", 0.1f, 0.0f);
    play_effect((GSuint)EffectID::ExplosionSmall, GSvector3::zero());
    world_->camera_shake(CameraShakeType::HandShake, 0.5f, 25.0f, false);
}
