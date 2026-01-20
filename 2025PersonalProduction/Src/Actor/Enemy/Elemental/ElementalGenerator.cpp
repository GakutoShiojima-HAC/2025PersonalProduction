#include "ElementalGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"
#include "Actor/Enemy/GetMyEnemyInfo.h"

ElementalGenerator::ElementalGenerator(const json& j, World* world) {
    world_ = world;

    GetMyEnemyInfo::get(my_info_, j);

    if (MyJson::is_object(j, "Param")) {
        auto& param = j["Param"];

        const std::string type = MyJson::get_string(param, "SpellType");
        if (type == "FireBall") info_.spell_type = ElementalSpellType::FireBall;
        else if (type == "ExplodeRoad") info_.spell_type = ElementalSpellType::ExplodeRoad;
    }
  
    if (MyJson::is_object(j, "Motion")) {
        auto& motion = j["Motion"];

        info_.motion_idle = MyJson::get_int(motion, "Idle");
        info_.motion_angry = MyJson::get_int(motion, "Angry");
        info_.motion_move = MyJson::get_int(motion, "Move");
        info_.motion_hurt = MyJson::get_int(motion, "Hurt");
        info_.motion_dead = MyJson::get_int(motion, "Dead");
        info_.motion_attack1 = MyJson::get_int(motion, "Attack1");
        info_.motion_attack2 = MyJson::get_int(motion, "Attack2");
        info_.motion_spell = MyJson::get_int(motion, "Spell");
    }
}

Actor* ElementalGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.damage = damage;

    Elemental* p = new Elemental{ world_, position, rotate, my_info, info_ };

    world_->add_character(p);
    return p;
}

bool ElementalGenerator::is_respawn() const {
    return false;
}
