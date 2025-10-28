#include "Engine/Core/Actor/ActorGenerator.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "Engine/Utils/MyJson.h"

#include "Actor/Player/PlayerGenerator.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemyGenerator.h"

namespace fs = std::experimental::filesystem;

const std::string FOLDER_PATH{ "Resource/Private/Behavior/Actor/" };

ActorGenerator::~ActorGenerator() {
    clear();
}

void ActorGenerator::load(World* world) {
    clear();

    // ��`����Ă���A�N�^�[�t�@�C����S�擾����
    std::vector<std::string> paths;
    for (const auto& entry : fs::recursive_directory_iterator(FOLDER_PATH)) {
        const fs::path& p = entry.path();
        if (fs::is_regular_file(p)) {
            const std::string file_ext = entry.path().extension().string();
            if (file_ext == ".json") paths.push_back(entry.path().string());
        }
    }
    if (paths.empty()) return;

    for (const auto& path : paths) {
        json j;
        if (!MyJson::is_json(path, j)) continue;
        const std::string key = MyJson::get_string(j, "Actor");
        const std::string name = MyJson::get_string(j, "Name");

        // 10/27 �����enum��map�����"else if"�A�ł̕������₷���A�\�[�X�t�@�C��1�̕ҏW��
        // �ǉ����������邽�߁A���̂܂܂ł����܂�
        if (key == PlayerGeneratorKey) data_[name] = new PlayerGenerator(j, world);
        else if (key == SimpleEnemyGeneratorKey) data_[name] = new SimpleEnemyGenerator(j, world);
    }
}

void ActorGenerator::generate(const std::string& json_file) {
    json j;
    if (!MyJson::is_json(json_file, j)) return;

    for (const auto& item : j["Actor"]) {
        // �L�[���擾
        const std::string key = MyJson::get_string(item, "Name");
        printf("generate: %s\n", key.c_str());
        
        // ���ʃp�����[�^���擾
        const GSvector3 position = GSvector3{ item["Position"][0], item["Position"][1], item["Position"][2] };
        const GSvector3 lookat = GSvector3{ item["LookAt"][0], item["LookAt"][1], item["LookAt"][2] };
        const int hp = MyJson::get_int(item, "HP");
        const int damage = MyJson::get_int(item, "Damage");

        // ����
        generate(key, position, lookat, hp, damage);
    }
}

void ActorGenerator::generate(const std::string& actor_key, const GSvector3& position, const GSvector3& lookat, int hp, int damage) {
    // ���݂��Ȃ��L�[�Ȃ琶�����Ȃ�
    auto it = data_.find(actor_key);
    if (it == data_.end()) return;

    // ����
    it->second->generate(position, lookat, hp, damage);
}

void ActorGenerator::clear() {
    for (auto& i : data_) {
        delete i.second;
        i.second = nullptr;
    }
    data_.clear();
}
