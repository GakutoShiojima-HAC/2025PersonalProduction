#include "CheckPoint.h"
#include "Engine/Core/Scene/SceneManager.h"

CheckPoint::CheckPoint(IWorld* world, const GSvector3& position, const GSvector3& rotate, float radius) {
    world_ = world;
    tag_ = ActorTag::None;
    name_ = "CheckPoint";

    transform_.position(position);
    transform_.eulerAngles(rotate);

    enable_collider_ = true;
    collider_ = BoundingSphere{ radius };
}

void CheckPoint::react(Actor& other) {
    if (other.tag() == ActorTag::Player) {
        world_->update_check_point(transform_.position(), transform_.eulerAngles());
        is_dead_ = true;
        enable_collider_ = false;
        return;
    }
}

void CheckPoint::die() {
    // ŠO•”‚©‚çíœ‚³‚¹‚È‚¢
}
