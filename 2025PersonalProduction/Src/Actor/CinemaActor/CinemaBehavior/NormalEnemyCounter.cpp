#include "NormalEnemyCounter.h"
#include "Engine/Core/World/IWorld.h"

NormalEnemyCounter::NormalEnemyCounter(IWorld* world, int enemy_count) : ICinemaBehavior(world) {
    int count{ 0 };
    for (const auto& actor : world_->get_all_actor()) {
        if (actor->tag() == ActorTag::Enemy) ++count;
    }
    counter_ = count - enemy_count;
}

void NormalEnemyCounter::update(float delta_time) {
    if (is_play_) return;

    int count{ 0 };
    for (const auto& actor : world_->get_all_actor()) {
        if (actor->tag() == ActorTag::Enemy) ++count;
    }

    // 通常エネミーが残っていないと判断出来たら
    if (count <= counter_) is_start_ = true;
}

void NormalEnemyCounter::draw_gui() const {
    if (is_play_) return;

    // TODO Enemy Count 
}
