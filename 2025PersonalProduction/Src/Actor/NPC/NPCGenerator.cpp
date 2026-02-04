#include "NPCGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"

NPCGenerator::NPCGenerator(const json& j, World* world) {
    world_ = world;

    data_.name = MyJson::get_string(j, "Name");

    if (!MyJson::is_object(j, "Param")) return;
    auto& param = j["Param"];

    data_.motion_idle = MyJson::get_int(param, "MotionIdle");
    data_.motion_action = MyJson::get_int(param, "MotionAction");
    data_.skinmesh = Assets::to_mesh_id(MyJson::get_string(param, "SkinMeshID"));
}

Actor* NPCGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    data_.timeline = MyJson::get_string(param, "Timeline");

    NPC* p = new NPC{ world_, position, rotate, data_ };

    world_->add_character(p);
    return p;
}

bool NPCGenerator::is_respawn() const {
    return false;
}
