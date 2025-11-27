#include "LichGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"
#include "Actor/Enemy/GetMyEnemyInfo.h"

LichGenerator::LichGenerator(const json& j, World* world) {
	world_ = world;

    GetMyEnemyInfo::get(my_info_, j);

    if (!MyJson::is_object(j, "Lottery")) return;
    auto& lottery = j["Lottery"];

    info_.attack_lottery_by_attack = MyJson::get_float(lottery, "Attack");
    info_.attack_lottery_by_spell = MyJson::get_float(lottery, "Spell");
}

Actor* LichGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.damage = damage;

    /*
        ŒÂ•Ê‚ÅÀs‚µ‚½‚¢ˆ—
    */

    Lich* p = new Lich{ world_, position, rotate, my_info, info_ };

    /*
        ŒÂ•Ê‚ÅÀs‚µ‚½‚¢ˆ—
    */

    world_->add_character(p);
    return p;
}

bool LichGenerator::is_respawn() const {
    return false;
}
