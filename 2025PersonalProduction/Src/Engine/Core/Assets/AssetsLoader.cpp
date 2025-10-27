#include "Engine/Core/Assets/AssetsLoader.h"
#include <fstream>
#include <vector>
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Utils/Folder.h"
#include "Assets.h"
#include "Lib/json.hpp"
using json = nlohmann::json;

const std::string ASSETS_PATH{ "Resource/Assets/" };
const std::vector<std::string> TAG{
    { "mesh" },     // 0
    { "skinmesh" }, // 1
    { "texture" },  // 2
    { "octree" },   // 3
    { "bgm" },      // 4
    { "se" },       // 5
    { "effect" }    // 6
};

bool AssetsLoader::load_by_json(const std::string& file_path, const std::string& name) {
    // jsonファイルがなければその時点で終了
    json j;
    std::ifstream file(file_path);
    if (!file.is_open()) return false;

    // jsonオブジェクトかどうか
    auto is_object = [](const json& json_object, const std::string& object_name) {
        return json_object.contains(object_name.c_str()) && json_object[object_name.c_str()].is_object();
    };

    // オブジェクトの番号から読み込み先を指定して読み込む
    auto object_load = [](LoadAssets* load_assets, size_t object_num, const std::string& to, const std::string& file_path) {
        switch (object_num) {
        case 0: // mesh
            load_assets->mesh.push_back({ Assets::to_mesh_id(to), file_path });
            break;
        case 1: // skinmesh
            load_assets->skinmesh.push_back({ Assets::to_mesh_id(to), file_path });
            break;
        case 2: // texture
            load_assets->texture.push_back({ Assets::to_texture_id(to), file_path });
            break;
        case 3: // octree
            load_assets->octree.push_back({ Assets::to_octree_id(to), file_path });
            break;
        case 4: // bgm
            load_assets->bgm.push_back({ Assets::to_bgm_id(to), file_path });
            break;
        case 5: // se
            load_assets->se.push_back({ Assets::to_se_id(to), file_path });
            break;
        case 6: // effect
            load_assets->effect.push_back({ Assets::to_effect_id(to), file_path });
            break;
        default:
            break;
        }
    };

    file >> j;
    LoadAssets* request = new LoadAssets{};
    request->name = name;

    // オブジェクトからファイルパスを取得する
    for (size_t i = 0; i < TAG.size(); ++i) {
        const std::string tag = TAG[i];
        // オブジェクトが存在しなければ次
        if (!is_object(j, tag)) continue;               // 9/19 ラムダ式と早期リターンでネストが二行浅くなった嬉しい

        // オブジェクト内の全ファイルパスを取得する
        for (const auto& obj : j[tag].items()) {
            const std::string to = obj.key();
            const std::string file_path = ASSETS_PATH + obj.value().get<std::string>();

            // オブジェクトの番号から読み込み先を指定して読み込む
            object_load(request, i, to, file_path);
        }
    }
    request->light_map = j.contains("LightMap") && j["LightMap"].is_string() ? j["LightMap"].get<std::string>() : "";
    request->reflection_probe = j.contains("ReflectionProbe") && j["ReflectionProbe"].is_string() ? j["ReflectionProbe"].get<std::string>() : "";

    // 読み込む
    AssetsManager::get_instance().load_asset(request);
    // requestのdeleteはAssetsManagerが行う
    
    // リクエストを送れたので成功とする
    return true;
}
