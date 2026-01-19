#include "VikingGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"
#include "Actor/Enemy/GetMyEnemyInfo.h"

VikingGenerator::VikingGenerator(const json& j, World* world) {
    world_ = world;

    GetMyEnemyInfo::get(my_info_, j);
}

Actor* VikingGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.damage = damage;

    /*
        ŒÂ•Ê‚ÅÀs‚µ‚½‚¢ˆ—
    */

    Viking* p = new Viking{ world_, position, rotate, my_info };

    /*
        ŒÂ•Ê‚ÅÀs‚µ‚½‚¢ˆ—
    */

    world_->add_character(p);
    return p;
}

bool VikingGenerator::is_respawn() const {
    return false;
}
