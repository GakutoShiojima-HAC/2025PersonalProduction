#include "LichGenerator.h"
#include "Engine/Core/World/World.h"
#include "Assets.h"

LichGenerator::LichGenerator(const json& j, World* world) {
	world_ = world;

    my_info_.name = MyJson::get_string(j, "Name");
    if (!MyJson::is_object(j, "Param")) return;
    auto& param = j["Param"];

    my_info_.type = MyJson::get_string(param, "Type");

    my_info_.move_speed = MyJson::get_float(param, "MoveSpeed");
    my_info_.skinmesh = Assets::to_mesh_id(MyJson::get_string(param, "SkinMeshID"));

    my_info_.search_fov = MyJson::get_float(param, "SearchFov");
    my_info_.search_length = MyJson::get_float(param, "SearchLength");

    my_info_.attack_radius = MyJson::get_float(param, "AttackRadius");
    my_info_.attack_detection_radius = MyJson::get_float(param, "AttackDetectionRadius");
    my_info_.attack_offset = GSvector3{ param["AttackOffset"][0], param["AttackOffset"][1], param["AttackOffset"][2] };

    my_info_.falter_rate = MyJson::get_float(param, "FalterRate");
    my_info_.critical_bone_num = MyJson::get_int(param, "CriticalBone");

    info_.attack_lottery_by_attack = MyJson::get_float(param, "LotteryByAttack");
    info_.attack_lottery_by_spell = MyJson::get_float(param, "LotteryBySpell");
    /*
        基礎データから生成に必要なデータを取得
    */
}

Actor* LichGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.attack_damage = damage;

    /*
        個別で実行したい処理
    */

    Lich* p = new Lich{ world_, position, rotate, my_info, info_ };

    /*
        個別で実行したい処理
    */

    world_->add_character(p);
    return p;
}

bool LichGenerator::is_respawn() const {
    return false;
}
