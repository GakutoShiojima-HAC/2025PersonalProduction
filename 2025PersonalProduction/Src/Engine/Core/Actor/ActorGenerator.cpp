#include "Engine/Core/Actor/ActorGenerator.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#include "Actor/StageTeleporter/StageTeleporterGenerator.h"
#include "Actor/Player/PlayerGenerator.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemyGenerator.h"
#include "Actor/Gimmick/Elevator/ElevatorGenerator.h"

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
        if (key == StageTereporterGeneratorKey) data_[name] = new StageTereporterGenerator(j, world);
        else if (key == PlayerGeneratorKey) data_[name] = new PlayerGenerator(j, world);
        else if (key == SimpleEnemyGeneratorKey) data_[name] = new SimpleEnemyGenerator(j, world);
        else if (key == ElevatorGeneratorKey) data_[name] = new ElevatorGenerator(j, world);
    }
}

void ActorGenerator::generate(const std::string& json_file) {
    json j;
    if (!MyJson::is_json(json_file, j)) return;

    for (const auto& item : j["Actor"]) {
        // キーを取得
        const std::string key = MyJson::get_string(item, "Name");
        
        // 共通パラメータを取得
        const GSvector3 position = MyJson::get_vector3(item, "Position");
        const GSvector3 lookat = MyJson::get_vector3(item, "LookAt"); 
        const int hp = MyJson::get_int(item, "HP");
        const int damage = MyJson::get_int(item, "Damage");
        json param;
        if (MyJson::is_object(item, "Param")) {
            param = item["Param"];
        }
        // 生成
        generate(key, position, lookat, hp, damage, param);
    }
}

void ActorGenerator::clear() {
    for (auto& i : data_) {
        delete i.second;
        i.second = nullptr;
    }
    data_.clear();

    generate_enemy_ = 0;
    generate_boss_ = 0;
}

int ActorGenerator::count_generate_enemy() const {
    return generate_enemy_;
}

int ActorGenerator::count_generate_boss() const {
    return generate_boss_;
}

void ActorGenerator::generate(const std::string& actor_key, const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) {
    // 存在しないキーなら生成しない
    auto it = data_.find(actor_key);
    if (it == data_.end()) return;

    // 生成
    Actor* actor = it->second->generate(position, lookat, hp, damage, param);

    // 生成された敵の数をカウント
    if (actor == nullptr || actor->tag() != ActorTag::Enemy) return;
    if (MyJson::get_boolean(param, "Boss", false)) ++generate_boss_;
    else ++generate_enemy_;
}
