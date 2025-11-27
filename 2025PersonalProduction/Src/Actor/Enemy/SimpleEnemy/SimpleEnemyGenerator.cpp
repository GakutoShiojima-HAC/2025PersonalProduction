#include "Actor/Enemy/SimpleEnemy/SimpleEnemyGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"
#include "Actor/Enemy/GetMyEnemyInfo.h"

SimpleEnemyGenerator::SimpleEnemyGenerator(const json& j, World* world) {
    world_ = world;

    GetMyEnemyInfo::get(my_info_, j);

    if (!MyJson::is_object(j, "Motion")) return;
    auto& motion = j["Motion"];

    info_.motion_idle = MyJson::get_int(motion, "Idle");
    info_.motion_move = MyJson::get_int(motion, "Move");
    info_.motion_attack = MyJson::get_int(motion, "Attack");
    info_.motion_hurt = MyJson::get_int(motion, "Hurt");
    info_.motion_dead = MyJson::get_int(motion, "Dead");
}

Actor* SimpleEnemyGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.damage = damage;
    Character* p = new SimpleEnemy{ world_, position, rotate, my_info, info_ };
    world_->add_character(p);
    return p;
}
