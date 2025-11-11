#include "Actor/StageTeleporter/StageTeleporterGenerator.h"
#include "Actor/StageTeleporter/StageTeleporter.h"

StageTereporterGenerator::StageTereporterGenerator(const json& j, World* world) {
    world_ = world;

    // json‚ÍŽg‚í‚È‚¢
}

void StageTereporterGenerator::generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) {
    world_->add_actor(new StageTereporter{ world_, position, lookat, MyJson::get_int(param, "Stage") });
}
