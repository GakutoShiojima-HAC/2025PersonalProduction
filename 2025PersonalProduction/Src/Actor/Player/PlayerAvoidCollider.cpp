#include "PlayerAvoidCollider.h"
#include "Actor/Player/Player.h"
#include "Engine/Core/World/IWorld.h"

constexpr float RADIUS_RATIO{ 4.5f };

PlayerAvoidCollider::PlayerAvoidCollider(IWorld* world, Player& owner) :
    owner_{ owner } {
    world_ = world;
    name_ = "PlayerAvoidCollider";
    tag_ = ActorTag::PlayerAvoid;
    // 追従
    transform_.position(owner.transform().position());
    transform_.rotation(owner.transform().rotation());
    transform_.setParent(&owner.transform());
    // 衝突判定を作成
    collider_ = BoundingSphere{ owner_.collider().radius * RADIUS_RATIO, GSvector3{ 0.0f, owner_.height() / 2.0f, 0.0f }};
}

void PlayerAvoidCollider::take_damage(Actor& other, const int damage) {
    // 再度起動されるまでエフェクトを出せなくなる
    enable_collider_ = false;
    // プレイヤーに通知
    Pawn* target = dynamic_cast<Pawn*>(&other);
    owner_.avoid_effect_start(target);
}

bool& PlayerAvoidCollider::enabled() {
    return enable_collider_;
}

void PlayerAvoidCollider::start_effect() {
    
}
