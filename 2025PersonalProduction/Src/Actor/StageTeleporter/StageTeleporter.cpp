#include "Actor/StageTeleporter/StageTeleporter.h"
#include "Engine/Core/Scene/SceneManager.h"
#include <any>
#include "GameConfig.h"
#include "Assets.h"

StageTereporter::StageTereporter(IWorld* world, const GSvector3& position, const GSvector3& rotate, int stage_id) {
    world_ = world;
    stage_id_ = stage_id;
    tag_ = ActorTag::Object;
    name_ = "StageTereporter";
    transform_.position(position);
    transform_.eulerAngles(rotate);

    // テレポートできるかどうか
    enable_collider_ = stage_id <= world_->game_save_data().get().stage + 1;

    if (enable_collider_) {
        collider_ = BoundingSphere{ 0.5f, GSvector3{ 0.0f, 1.0f, 0.0f } };
    }
}

void StageTereporter::update(float delta_time) {
    is_touch_ = false;  // reset
}

void StageTereporter::late_update(float delta_time) {
    if (is_touch_) {
        timer_ += delta_time / cFPS;

        if (timer_ > 0.5f && !is_dead_) {
            std::any data = stage_id_;
            SceneManager::get_instance().send_message(SceneTag::Game, "RequestTereport", data);
            is_dead_ = true;
            return;
        }
    }
    else {
        timer_ = 0.0f;
    }
}

void StageTereporter::draw() const {
    if (enable_collider_) {
        glPushMatrix();
        glMultMatrixf(transform_.localToWorldMatrix());
        gsDrawMesh((GSuint)MeshID::Tereporter);
        glPopMatrix();
    }
}

void StageTereporter::react(Actor& other) {
    if (other.tag() == ActorTag::Player) {
        is_touch_ = true;
    }
}

void StageTereporter::die() {
    // 外部から削除させない
}
