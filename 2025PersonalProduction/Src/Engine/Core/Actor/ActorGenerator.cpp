#include "Engine/Core/Actor/ActorGenerator.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#include "Actor/StageTeleporter/StageTeleporterGenerator.h"
#include "Actor/Gimmick/CheckPoint/CheckPointGenerator.h"
#include "Actor/CinemaActor/CinemaActorGenerator.h"
#include "Actor/Player/PlayerGenerator.h"
#include "Actor/Gimmick/Elevator/ElevatorGenerator.h"
#include "Actor/Gimmick/Barrier/BarrierGenerator.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemyGenerator.h"
#include "Actor/Enemy/Lich/LichGenerator.h"
#include "Tutorial/Enemy/TutorialFacilitatorGenerator.h"
#include "Actor/Gimmick/TreasureChest/TreasureChestGenerator.h"

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
        // 11/18 もしかしたらumapのkey部分にgeneratorkeyを入れとくことで処理減ったりする？(generatekey, pair<actornamekey, generator>)
        if (key == StageTereporterGeneratorKey) add_generator(name, new StageTereporterGenerator(j, world));
        else if (key == CheckPointGeneratorKey) add_generator(name, new CheckPointGenerator{ j, world });
        else if (key == CinemaActorGeneratorKey) add_generator(name, new CinemaActorGenerator(j, world));
        else if (key == PlayerGeneratorKey) add_generator(name, new PlayerGenerator(j, world));
        else if (key == ElevatorGeneratorKey) add_generator(name, new ElevatorGenerator(j, world));
        else if (key == BarrierGeneratorKey) add_generator(name, new BarrierGenerator(j, world));
        else if (key == SimpleEnemyGeneratorKey) add_generator(name, new SimpleEnemyGenerator(j, world));
        else if (key == LichGeneratorKey) add_generator(name, new LichGenerator{ j, world });
        else if (key == TutorialFacilitatorGeneratorKey) add_generator(name, new TutorialFacilitatorGenerator{ j, world });
        else if (key == TreasureChestGeneratorKey) add_generator(name, new TreasureChestGenerator{ j, world });
    }
}

void ActorGenerator::generate(const std::string& json_file) {
    json j;
    if (!MyJson::is_json(json_file, j)) return;

    for (const auto& item : j["Actor"]) {
        // キーを取得
        const std::string key = MyJson::get_string(item, "Name");

        // プレイヤーを生成するかどうか
        if (key == "Player") {
            ++generate_player_;
            if (generate_player_ > 1) continue;
        }

        // 共通パラメータを取得
        const GSvector3 position = MyJson::get_vector3(item, "Position");
        const GSvector3 rotate = MyJson::get_vector3(item, "Rotate");
        const int hp = MyJson::get_int(item, "HP");
        const int damage = MyJson::get_int(item, "Damage");
        json param;
        if (MyJson::is_object(item, "Param")) {
            param = item["Param"];
        }

        // 生成
        generate(key, position, rotate, hp, damage, param);
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
    generate_player_ = 0;
}

void ActorGenerator::clear_no_respawn() {
    for (auto it = data_.begin(); it != data_.end(); ) {
        // リスポーンするならスキップ
        if (it->second != nullptr && it->second->is_respawn()) {
            ++it;
            continue;
        }

        delete it->second;
        it->second = nullptr;
        it = data_.erase(it);
    }

    generate_enemy_ = 0;
    generate_boss_ = 0;
    generate_player_ = 0;
}

int ActorGenerator::count_generate_enemy() const {
    return generate_enemy_;
}

int ActorGenerator::count_generate_boss() const {
    return generate_boss_;
}

void ActorGenerator::add_generator(const std::string& name, IActorGenerator* generator) {
    auto it = data_.find(name);
    if (it != data_.end()) delete it->second;
    data_[name] = generator;
}

void ActorGenerator::generate(const std::string& actor_key, const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    // 存在しないキーなら生成しない
    auto it = data_.find(actor_key);
    if (it == data_.end()) return;

    // 生成
    Actor* actor = it->second->generate(position, rotate, hp, damage, param);

    // 生成された敵の数をカウント
    if (actor == nullptr || actor->tag() != ActorTag::Enemy) return;
    if (MyJson::get_boolean(param, "Boss", false)) ++generate_boss_;
    else ++generate_enemy_;
}
