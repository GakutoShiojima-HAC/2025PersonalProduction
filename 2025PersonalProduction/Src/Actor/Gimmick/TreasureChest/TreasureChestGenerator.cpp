#include "TreasureChestGenerator.h"
#include "TreasureChest.h"
#include "Engine/Core/World/World.h"
#include "Item/ItemData.h"

TreasureChestGenerator::TreasureChestGenerator(const json& j, World* world) {
	world_ = world;
}

Actor* TreasureChestGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    ItemData::Data data;
    data.type = ItemData::to_type(MyJson::get_string(param, "Type"));
    data.id = MyJson::get_int(param, "ID");
    
    TreasureChest* p = new TreasureChest{ world_, position, rotate, data };

    world_->add_actor(p);
    return p;
}

bool TreasureChestGenerator::is_respawn() const {
    return false;
}
