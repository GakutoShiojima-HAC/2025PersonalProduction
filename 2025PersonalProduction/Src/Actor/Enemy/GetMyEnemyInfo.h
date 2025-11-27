#ifndef GET_MY_ENEMY_INFO_H_
#define GET_MY_ENEMY_INFO_H_

#include "MyEnemy.h"
#include "Engine/Utils/MyJson.h"
#include "Assets.h"

namespace GetMyEnemyInfo {
    /// <summary>
    /// MyEnemyInfo用データをjsonから取得
    /// </summary>
    /// <param name="info">= 格納先</param>
    /// <param name="j">= jsonデータ</param>
    inline void get(MyEnemyInfo& info, const json& j) {
        info.name = MyJson::get_string(j, "Name");

        if (!MyJson::is_object(j, "Param")) return;
        auto& param = j["Param"];

        info.pawn_type = MyJson::get_string(param, "Type", "Normal");
        info.skinmesh = Assets::to_mesh_id(MyJson::get_string(param, "SkinMeshID"));
        info.move_speed = MyJson::get_float(param, "MoveSpeed", 0.0f);
        info.search_fov = MyJson::get_float(param, "SearchFov");
        info.search_length = MyJson::get_float(param, "SearchLength");
        info.critical_bone_num = MyJson::get_int(param, "CriticalBone", 0);
        info.falter_rate = MyJson::get_float(param, "FalterRate", 1.0f);
        info.ui_height = MyJson::get_float(param, "UIHeight", 0.0f);

        // 全ての攻撃モーション
        for (const auto& obj : j["AttackEvent"]) {
            MyEnemyAttackData data;

            GSuint motion = MyJson::get_int(obj, "MotionID", 0);
            data.start_time = MyJson::get_float(obj, "StartTime");
            data.attack_time = MyJson::get_float(obj, "AttackTime");
            data.offset = MyJson::get_vector3(obj, "Offset");
            data.radius = MyJson::get_float(obj, "Radius");
            data.damage = MyJson::get_int(obj, "Damage", 0);
            data.detection_radius = MyJson::get_float(obj, "DetectionRadius");
            data.bone = MyJson::get_int(obj, "Bone", 0);

            info.attack_data[motion] = data;
        }
    }
}
#endif
