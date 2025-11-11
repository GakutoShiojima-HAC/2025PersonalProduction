#include "Actor/Player/PlayerGenerator.h"
#include "Engine/Core/World/World.h"
#include "Camera/PlayerCamera.h"
#include "Assets.h"

PlayerGenerator::PlayerGenerator(const json& j, World* world) {
    world_ = world;

    // モーションを取得するラムダ式
    auto get_motion = [](const json& json_object) {
        std::vector<PlayerGenerateAttackColliderEvent> events;
        // 発生タイミング分回す
        for (const auto& obj : json_object) {
            PlayerGenerateAttackColliderEvent event;
            event.offset = GSvector3{ obj["Offset"][0], obj["Offset"][1], obj["Offset"][2] };
            event.radius = MyJson::get_float(obj, "Radius");
            event.time = MyJson::get_float(obj, "Time");
            events.push_back(event);
        }

        return events;
    };

    auto get_effect = [](const json& json_object) {
        std::vector <PlayerGenerateEffectEvent> events;
        // 発生タイミング分回す
        for (const auto& obj : json_object) {
            PlayerGenerateEffectEvent event;
            event.effect_id = Assets::to_effect_id(MyJson::get_string(obj, "Effect"));
            event.offset = GSvector3{ obj["Offset"][0], obj["Offset"][1], obj["Offset"][2] };
            event.rotate = GSvector3{ obj["Rotate"][0], obj["Rotate"][1], obj["Rotate"][2] };
            event.scale = GSvector3{ obj["Scale"][0], obj["Scale"][1], obj["Scale"][2] };
            event.time = MyJson::get_float(obj, "Time");
            event.speed = MyJson::get_float(obj, "Speed", 1.0f);
            event.enabled = (GSuint)EffectID::NONE != event.effect_id;  // 未定義エフェクトでなければ生成
            events.push_back(event);

        }
        return events;
    };

    // 通常攻撃を取得
    for (const auto& obj : j["Attack"]) {
        // パラメータを取得
        PlayerAttackParam param;
        param.damage = MyJson::get_int(obj, "Damage");
        param.next_start = MyJson::get_float(obj, "NextInputStart");
        param.next_end = MyJson::get_float(obj, "NextInputEnd");
        info_.attack_param.push_back(param);
        // モーションを取得
        info_.attack_event.push_back(get_motion(obj["GenerateCollider"]));
        // エフェクトを取得
        info_.attack_effect_event.push_back(get_effect(obj["GenerateEffect"]));
    }
    
    // スキルを取得
    {
        const auto& obj = j["Skill"];
        info_.skill_damage = MyJson::get_int(obj, "Damage");
        // モーションを取得
        info_.skill_event = get_motion(obj["GenerateCollider"]);
        // エフェクトを取得
        info_.skill_effect_event = get_effect(obj["GenerateEffect"]);
    }
    // 回避攻撃を取得
    {
        const auto& obj = j["AvoidAttack"];
        info_.avoid_attack_damage = MyJson::get_int(obj, "Damage");
        // モーションを取得
        info_.avoid_attack_event = get_motion(obj["GenerateCollider"]);
        // エフェクトを取得
        info_.avoid_attack_effect_event = get_effect(obj["GenerateEffect"]);
    }
    // 回避成功攻撃を取得
    {
        const auto& obj = j["AvoidSuccessAttack"];
        info_.avoid_success_attack_damage = MyJson::get_int(obj, "Damage");
        // モーションを取得
        info_.avoid_success_attack_event = get_motion(obj["GenerateCollider"]);
        // エフェクトを取得
        info_.avoid_success_attack_effect_event = get_effect(obj["GenerateEffect"]);
    }
    // 回避成功スキルを取得
    {
        const auto& obj = j["AvoidSuccessSkill"];
        info_.avoid_success_skill_damage = MyJson::get_int(obj, "Damage");
        // モーションを取得
        info_.avoid_success_skill_event = get_motion(obj["GenerateCollider"]);
        // エフェクトを取得
        info_.avoid_success_skill_effect_event = get_effect(obj["GenerateEffect"]);
    }
}

void PlayerGenerator::generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) {
    // 生成は一体のみ
    if (!can_generate_) return;

    // プレイヤーカメラを取得
    PlayerCamera* camera = dynamic_cast<PlayerCamera*>(world_->find_camera(CameraTag::Player));

    // 生成
    world_->add_character(new Player{ world_, position, lookat, camera, info_ });
    can_generate_ = false;
}
