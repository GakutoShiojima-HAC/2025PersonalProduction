#include "Actor/StageTeleporter/StageTeleporterGenerator.h"
#include "Actor/StageTeleporter/StageTeleporter.h"

StageTereporterGenerator::StageTereporterGenerator(const json& j, World* world) {
    world_ = world;

    // json‚Íg‚í‚È‚¢
}

Actor* StageTereporterGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    Actor* p = new StageTereporter{ world_, position, rotate, MyJson::get_int(param, "Stage") };
    world_->add_actor(p);
    return p;
}
