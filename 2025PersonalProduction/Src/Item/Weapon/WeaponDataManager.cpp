#include "Item/Weapon/WeaponDataManager.h"
#include "Assets.h"
#include "Engine/Utils/MyJson.h"

const std::string FILE_PATH{ "/weapon.json" };

WeaponDataManager::~WeaponDataManager() {
    clear();
}

void WeaponDataManager::load(const std::string& folder_path) {
    clear();

    // jsonファイルがなければその時点で終了
    json j;
    if (!MyJson::is_json(folder_path + FILE_PATH, j)) return;

    // バージョンを取得
    if (MyJson::is_object(j, "version")) version_ = j["version"];

    // データを取得
    for (const auto& item : j["data"]) {
        WeaponData::Data data;
        data.id = MyJson::get_int(item, "id");
        data.name = MyJson::get_string(item, "name");
        data.description = MyJson::get_string(item, "description");
        data.texture = Assets::to_texture_id(MyJson::get_string(item, "texture"));
        data.mesh = Assets::to_mesh_id(MyJson::get_string(item, "mesh"));
        data.type = (WeaponType)WeaponData::to_id(MyJson::get_string(item, "type"));
        data.damage = MyJson::get_int(item, "damage");

        // IDが既に存在しても上書き格納
        data_[data.id] = data;
    }
}

void WeaponDataManager::clear() {
    // メモリごと開放
    std::unordered_map<int, WeaponData::Data>().swap(data_);
}

WeaponData::Data WeaponDataManager::get(int id) const {
    auto it = data_.find(id);
    if (it != data_.end())  return it->second;
    return WeaponData::empty_data();
}

int WeaponDataManager::version() const {
    return version_;
}
