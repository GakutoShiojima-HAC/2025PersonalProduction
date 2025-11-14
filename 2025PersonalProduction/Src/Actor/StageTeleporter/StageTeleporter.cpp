#include "Actor/StageTeleporter/StageTeleporter.h"
#include "Engine/Core/Scene/SceneManager.h"
#include <any>

StageTereporter::StageTereporter(IWorld* world, const GSvector3& position, const GSvector3& lookat, int stage_id) {
    world_ = world;
    stage_id_ = stage_id;
    tag_ = ActorTag::Object;
    name_ = "StageTereporter";
    transform_.position(position);

    // テレポートできるかどうか
    enable_collider_ = world_->game_save_data().get().stage <= stage_id - 1;

    if (enable_collider_) {
        collider_ = BoundingSphere{ 0.5f, GSvector3{ 0.0f, 1.0f, 0.0f } };
    }
}

void StageTereporter::react(Actor& other) {
    if (other.tag() == ActorTag::Player) {
        std::any data = stage_id_;
        SceneManager::get_instance().send_message(SceneTag::Game, "RequestTereport", data);
        die();
    }
}

void StageTereporter::die() {
    // 外部から削除させない
}
