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

    // 定義されているアクターファイルを全取得する
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

        // 10/27 下手にenumやmap作るより"else if"連打の方が見やすく、ソースファイル1つの編集で
        // 追加が完結するため、このままでいきます
        if (key == PlayerGeneratorKey) data_[name] = new PlayerGenerator(j, world);
        else if (key == SimpleEnemyGeneratorKey) data_[name] = new SimpleEnemyGenerator(j, world);
    }
}

void ActorGenerator::generate(const std::string& json_file) {
    json j;
    if (!MyJson::is_json(json_file, j)) return;

    for (const auto& item : j["Actor"]) {
        // キーを取得
        const std::string key = MyJson::get_string(item, "Name");
        printf("generate: %s\n", key.c_str());
        
        // 共通パラメータを取得
        const GSvector3 position = GSvector3{ item["Position"][0], item["Position"][1], item["Position"][2] };
        const GSvector3 lookat = GSvector3{ item["LookAt"][0], item["LookAt"][1], item["LookAt"][2] };
        const int hp = MyJson::get_int(item, "HP");
        const int damage = MyJson::get_int(item, "Damage");

        // 生成
        generate(key, position, lookat, hp, damage);
    }
}

void ActorGenerator::generate(const std::string& actor_key, const GSvector3& position, const GSvector3& lookat, int hp, int damage) {
    // 存在しないキーなら生成しない
    auto it = data_.find(actor_key);
    if (it == data_.end()) return;

    // 生成
    it->second->generate(position, lookat, hp, damage);
}

void ActorGenerator::clear() {
    for (auto& i : data_) {
        delete i.second;
        i.second = nullptr;
    }
    data_.clear();
}
