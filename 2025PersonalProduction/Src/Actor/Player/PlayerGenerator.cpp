#include "Actor/Player/PlayerGenerator.h"
#include "Engine/Core/World/World.h"
#include "Camera/PlayerCamera.h"

PlayerGenerator::PlayerGenerator(const json& j, World* world) {
    world_ = world;

    // ���[�V�������擾���郉���_��
    auto get_motion = [](const json& json_object) {
        std::vector<PlayerGenerateAttackColliderEvent> events;
        // �����^�C�~���O����
        for (const auto& obj : json_object) {
            PlayerGenerateAttackColliderEvent event;
            event.offset = GSvector3{ obj["Offset"][0], obj["Offset"][1], obj["Offset"][2] };
            event.radius = MyJson::get_float(obj, "Radius");
            event.time = MyJson::get_float(obj, "Time");
            events.push_back(event);
        }

        return events;
    };

    // �ʏ�U�����擾
    for (const auto& obj : j["Attack"]) {
        // �p�����[�^���擾
        PlayerAttackParam param;
        param.damage = MyJson::get_int(obj, "Damage");
        param.next_start = MyJson::get_float(obj, "NextInputStart");
        param.next_end = MyJson::get_float(obj, "NextInputEnd");
        info_.attack_param.push_back(param);
        // ���[�V�������擾
        info_.attack_event.push_back(get_motion(obj["GenerateCollider"]));
    }
    
    // �X�L�����擾
    {
        const auto& obj = j["Skill"];
        info_.skill_damage = MyJson::get_int(obj, "Damage");
        // ���[�V�������擾
        info_.skill_event = get_motion(obj["GenerateCollider"]);
    }
    // ����U�����擾
    {
        const auto& obj = j["AvoidAttack"];
        info_.avoid_attack_damage = MyJson::get_int(obj, "Damage");
        // ���[�V�������擾
        info_.avoid_attack_event = get_motion(obj["GenerateCollider"]);
    }
    // ��𐬌��U�����擾
    {
        const auto& obj = j["AvoidSuccessAttack"];
        info_.avoid_success_attack_damage = MyJson::get_int(obj, "Damage");
        // ���[�V�������擾
        info_.avoid_success_attack_event = get_motion(obj["GenerateCollider"]);
    }
    // ��𐬌��X�L�����擾
    {
        const auto& obj = j["AvoidSuccessSkill"];
        info_.avoid_success_skill_damage = MyJson::get_int(obj, "Damage");
        // ���[�V�������擾
        info_.avoid_success_skill_event = get_motion(obj["GenerateCollider"]);
    }
}

void PlayerGenerator::generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage) {
    // �����͈�̂̂�
    if (!can_generate_) return;

    // �v���C���[�J�������擾
    PlayerCamera* camera = dynamic_cast<PlayerCamera*>(world_->find_camera(CameraTag::Player));

    // ����
    world_->add_character(new Player{ world_, position, lookat, camera, info_ });
    can_generate_ = false;
}
