#include "Item/Inventory.h"
#include "Engine/Utils/Check.h"
#include "Engine/Utils/Folder.h"
#include "Item/ItemDataManager.h"
#include "Engine/Utils/MyJson.h"

// ファイルパスから先のファイルパス
const std::string FILE_PATH{ "/inventory.json" };

void Inventory::load(const std::string& folder_path) {
    ItemDataManager& item_data = ItemDataManager::get_instance();

    // jsonファイルがなければ初期装備にして終了
    json j;
    if (!MyJson::is_json(folder_path + FILE_PATH, j)) return;

    // インベントリを展開
    if (MyJson::is_object(j, "Inventory")) {
        const auto& inventory_obj = j["Inventory"];
        for (const auto& type_obj : inventory_obj.items()) {
            const std::string& type_name = type_obj.key();
            const ItemType type = ItemData::to_type(type_name);

            // 保存しなくてよい持ち物はスキップ
            if (MyLib::is_in(type, ItemType::NONE, ItemType::MAX_SIZE)) continue;

            // オブジェクトを取得
            for (const auto& item : type_obj.value()) {
                std::vector<int> row;   // 0がID 1が個数
                for (const auto& v : item) row.push_back(v);
                // 値がなければスキップ
                if (row.size() < 2) continue;
                // IDが見つからなければスキップ
                if (item_data.is_empty(ItemData::Data{ type, row[0] })) continue;
                // 追加
                inventory_[type].push_back(InventorySlot{ row[0], row[1] });
            }
        }
    }

    // TODO 武器データのバージョンが一致していなかったら...？
    
    // 現在の武器
    int weapon_id = MyJson::get_int(j, "CurrentWeaponID");
    current_weapon_ = item_data.get_weapon(weapon_id);
}

void Inventory::save(const std::string& folder_path) {
    ordered_json j;

    // 武器データのバージョン
    j["weapon_version"] = ItemDataManager::get_instance().weapon_version();
    // 使っていた武器
    j["CurrentWeaponID"] = current_weapon_.id;

    // 全体の持ち物
    for (const auto& item : inventory_) {
        // 保存しなくてよい持ち物はスキップ
        ItemType type = item.first;
        if (MyLib::is_in(type, ItemType::NONE, ItemType::MAX_SIZE)) continue;

        // オブジェクト名
        const std::string type_string = ItemData::to_string(type);
        ordered_json data;
        // ID・個数で格納
        for (const auto& v : item.second) {
            data[type_string].push_back({ v.id, v.count }); // 0がID 1が個数
        }
        j["Inventory"] = data;
    }

    // ファイル保存
    MyLib::write_to_file(folder_path + FILE_PATH, j);
}

void Inventory::clear() {
    std::unordered_map<ItemType, std::vector<InventorySlot>>().swap(inventory_);

    current_weapon_ = WeaponData::empty_data();
}

bool Inventory::add(ItemType type, int id, int count) {
    if (MyLib::is_in(type, ItemType::NONE, ItemType::MAX_SIZE)) return false;
    if (!is_in_id(type, id)) return false;

    // もし武器を持っていなく、武器だったら自動装備
    if (type == ItemType::Weapon && current_weapon_.is_empty()) {
        current_weapon_ = ItemDataManager::get_instance().get_weapon(id);
    }

    // 種類がなければ新しく追加
    if (inventory_.find(type) == inventory_.end()) {
        inventory_[type].push_back(InventorySlot{ id, count });
        return true;
    }

    // 既に登録されているIDなら個数だけ増加
    for (auto& item : inventory_[type]) {
        if (id == item.id) {
            item.count += count;
            return true;
        }
    }

    // 種類は登録されているがIDが未登録
    inventory_[type].push_back(InventorySlot{ id, count });
    return true;
}

bool Inventory::add(ItemData::Data data, int count) {
    return add(data.type, data.id, count);
}

bool Inventory::can_remove(ItemType type, int id, int count, bool deleted) {
    // なければ消せない
    if (MyLib::is_in(type, ItemType::NONE, ItemType::MAX_SIZE)) return false;
    if (!is_in_id(type, id)) return false;
    if (inventory_.find(type) == inventory_.end()) return false;

    // 使用中かどうか
    auto is_using = [this](ItemType type, int id) {
        switch (type) {
        case ItemType::Weapon: return current_weapon_.id == id;
        default: return false;
        }
        return false;
    };

    // 対象データ
    int remove_count = count;
    bool is_remove = false;

    // 登録済みか検索
    for (auto& item : inventory_[type]) {
        // 登録済みなら
        if (id == item.id) {
            // 使用中ならその個数分減らすように値を修正
            int current = item.count - is_using(type, id) ? 1 : 0;
            // 消せないなら終了
            if (current < item.count) return false;

            if (deleted) {
                remove_count = current;
                is_remove = true;
                break;
            }
            else {
                return true;
            }
        }
    }

    if (is_remove) remove(type, id, remove_count);
    return is_remove;
}

bool Inventory::can_remove(ItemData::Data data, int count, bool deleted) {
    return can_remove(data.type, data.id, count, deleted);
}

const WeaponData::Data& Inventory::weapon() const {
    return current_weapon_;
}

bool Inventory::is_in_id(ItemType type, int id) const {
    switch (type) {
    case ItemType::Weapon: return id > (int)WeaponType::NONE && id < (int)WeaponType::MAX_SIZE;
    default: return false;
    }
    return false;
}

void Inventory::remove(ItemType type, int id, int count) {
    // なければ消せない
    if (MyLib::is_in(type, ItemType::NONE, ItemType::MAX_SIZE)) return;
    if (!is_in_id(type, id)) return;
    if (inventory_.find(type) == inventory_.end()) return;
    if (count <= 0) return;

    for (auto i = inventory_[type].begin(); i != inventory_[type].end(); ) {
        if (i->id == id) {
            i->count -= count;
            if (i->count <= 0) i = inventory_[type].erase(i);
            break;  // 同一IDはほかに存在しないので終了
        }
        else {
            ++i;
        }
    }
}
