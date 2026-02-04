#include "HealGenerator.h"
#include "Engine/Core/World/World.h"
#include "Actor/Gimmick/Heal/Heal.h"
#include "Assets.h"

HealGenerator::HealGenerator(const json& j, World* world) {
    world_ = world;
}

Actor* HealGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    GSuint mesh = Assets::to_mesh_id(MyJson::get_string(param, "Mesh"));
    int value = MyJson::get_int(param, "Value");

    Heal* p = new Heal{ world_, position, rotate, mesh, value };

    world_->add_actor(p);
    return p;
}

bool HealGenerator::is_respawn() const {
    return false;
}
