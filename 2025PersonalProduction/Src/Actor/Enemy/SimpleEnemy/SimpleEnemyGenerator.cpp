#include "Actor/Enemy/SimpleEnemy/SimpleEnemyGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"

SimpleEnemyGenerator::SimpleEnemyGenerator(const json& j, World* world) {
    world_ = world;

    my_info_.name = MyJson::get_string(j, "Name");
    if (!MyJson::is_object(j, "Param")) return;
    auto& param = j["Param"];

    my_info_.hp_height = MyJson::get_float(param, "HPHeight");

    my_info_.type = MyJson::get_string(param, "Type");

    my_info_.move_speed = MyJson::get_float(param, "MoveSpeed");
    my_info_.skinmesh = Assets::to_mesh_id(MyJson::get_string(param, "SkinMeshID"));

    my_info_.search_fov = MyJson::get_float(param, "SearchFov");
    my_info_.search_length = MyJson::get_float(param, "SearchLength");

    my_info_.attack_event_time = MyJson::get_float(param, "AttackEventTime");
    my_info_.attack_radius = MyJson::get_float(param, "AttackRadius");
    my_info_.attack_detection_radius = MyJson::get_float(param, "AttackDetectionRadius");
    my_info_.attack_offset = GSvector3{ param["AttackOffset"][0], param["AttackOffset"][1], param["AttackOffset"][2] };

    my_info_.falter_rate = MyJson::get_float(param, "FalterRate");

    my_info_.critical_bone_num = MyJson::get_int(param, "CriticalBone");

    if (!MyJson::is_object(param, "Motion")) return;
    auto& motion = param["Motion"];

    info_.motion_idle = MyJson::get_int(motion, "Idle");
    info_.motion_move = MyJson::get_int(motion, "Move");
    info_.motion_attack = MyJson::get_int(motion, "Attack");
    info_.motion_hurt = MyJson::get_int(motion, "Hurt");
    info_.motion_dead = MyJson::get_int(motion, "Dead");

}

Actor* SimpleEnemyGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.attack_damage = damage;
    Character* p = new SimpleEnemy{ world_, position, rotate, my_info, info_ };
    world_->add_character(p);
    return p;
}
