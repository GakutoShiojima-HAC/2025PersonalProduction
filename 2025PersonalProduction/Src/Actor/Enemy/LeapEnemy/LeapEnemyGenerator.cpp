#include "LeapEnemyGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"
#include "Actor/Enemy/GetMyEnemyInfo.h"

LeapEnemyGenerator::LeapEnemyGenerator(const json& j, World* world) {
    world_ = world;

    GetMyEnemyInfo::get(my_info_, j);

    if (MyJson::is_object(j, "Motion")) {
        auto& motion = j["Motion"];

        info_.motion_idle = MyJson::get_int(motion, "Idle");
        info_.motion_move = MyJson::get_int(motion, "Move");
        info_.motion_hurt = MyJson::get_int(motion, "Hurt");
        info_.motion_dead = MyJson::get_int(motion, "Dead");
        info_.motion_attack = MyJson::get_int(motion, "Attack");
        info_.motion_leap_start = MyJson::get_int(motion, "LeapStart");
        info_.motion_leaping = MyJson::get_int(motion, "Leaping");
        info_.motion_leapend = MyJson::get_int(motion, "LeapEnd");
    }
}

Actor* LeapEnemyGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.damage = damage;

    LeapEnemy* p = new LeapEnemy{ world_, position, rotate, my_info, info_ };

    world_->add_character(p);
    return p;
}

bool LeapEnemyGenerator::is_respawn() const {
    return false;
}
