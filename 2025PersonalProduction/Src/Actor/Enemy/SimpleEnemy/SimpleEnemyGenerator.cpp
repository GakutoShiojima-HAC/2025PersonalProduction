#include "Actor/Enemy/SimpleEnemy/SimpleEnemyGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"

SimpleEnemyGenerator::SimpleEnemyGenerator(const json& j, World* world) {
    world_ = world;

    info_.name = MyJson::get_string(j, "Name");
    if (!MyJson::is_object(j, "Param")) return;
    auto param = j["Param"];

    info_.height = MyJson::get_float(param, "Height");
    info_.foot_offset = MyJson::get_float(param, "FootOffset");
    info_.radius = MyJson::get_float(param, "Radius");

    info_.move_speed = MyJson::get_float(param, "MoveSpeed");
    info_.skinmesh = Assets::to_mesh_id(MyJson::get_string(param, "SkinMeshID"));

    info_.search_fov = MyJson::get_float(param, "SearchFov");
    info_.search_length = MyJson::get_float(param, "SearchLength");

    info_.attack_event_time = MyJson::get_float(param, "AttackEventTime");
    info_.attack_radius = MyJson::get_float(param, "AttackRadius");
    info_.attack_detection_radius = MyJson::get_float(param, "AttackDetectionRadius");
    info_.attack_offset = GSvector3{ param["AttackOffset"][0], param["AttackOffset"][1], param["AttackOffset"][2] };

    info_.falter_rate = MyJson::get_float(param, "FalterRate");

    if (!MyJson::is_object(param, "Motion")) return;
    auto motion = param["Motion"];

    info_.motion_idle = MyJson::get_int(motion, "Idle");
    info_.motion_move = MyJson::get_int(motion, "Move");
    info_.motion_attack = MyJson::get_int(motion, "Attack");
    info_.motion_hurt = MyJson::get_int(motion, "Hurt");
    info_.motion_dead = MyJson::get_int(motion, "Dead");

}

void SimpleEnemyGenerator::generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) {
    SimpleEnemyInfo info = info_;
    info.hp = hp;
    info.attack_damage = damage;
    world_->add_character(new SimpleEnemy{ world_, position, lookat, info });
}
