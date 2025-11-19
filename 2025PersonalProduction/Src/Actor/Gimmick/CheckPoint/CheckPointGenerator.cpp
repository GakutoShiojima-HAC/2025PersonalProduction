#include "CheckPointGenerator.h"
#include "Engine/Core/World/World.h"
#include "Actor/Gimmick/CheckPoint/CheckPoint.h"

CheckPointGenerator::CheckPointGenerator(const json& j, World* world) {
    world_ = world;

    // json‚Íg‚í‚È‚¢
}

Actor* CheckPointGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    Actor* p = new CheckPoint{ world_, position, rotate, MyJson::get_float(param, "Radius") };
    world_->add_actor(p);
    return p;
}
