#include "Item/Inventory.h"
#include "Engine/Utils/Check.h"
#include "Item/ItemDataManager.h"

// ファイルパスから先のファイルパス
const std::string FILE_PATH{ "/inventory.json" };

void Inventory::load(const json& j) {
    ItemDataManager& item_data = ItemDataManager::get_instance();

    if (MyJson::is_object(j, "Items")) {
        const auto& items_obj = j["Items"];
        for (const auto& type_obj : items_obj.items()) {
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
    if (MyJson::is_object(j, "CurrentID")) {
        const auto& current_obj = j["CurrentID"];
        for (const auto& type_obj : current_obj.items()) {
            const std::string& type_name = type_obj.key();
            const ItemType type = ItemData::to_type(type_name);
            // 保存しなくてよい持ち物はスキップ
            if (MyLib::is_in(type, ItemType::NONE, ItemType::MAX_SIZE)) continue;
            // 追加
            int current_id = MyJson::get_int(current_obj, type_name, -1);
            switch (type) {
            case ItemType::Weapon:
                current_weapon_ = item_data.get_weapon(current_id); break;
            default:
                break;
            }
        }
    }
}

ordered_json Inventory::save_object() const {
    ordered_json j;

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
        j["Items"] = data;
    }
    // 現在のID
    {
        ordered_json current;
        for (size_t i = 0; i < (size_t)ItemType::MAX_SIZE; ++i) {
            const std::string type_string = ItemData::to_string(ItemType(i));
            switch (ItemType(i)) {
            case ItemType::Weapon:
                current[type_string] = current_weapon_.id;
                break;
            default:
                break;
            }
        }
        j["CurrentID"] = current;
    }

    return j;
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

// TODO 少しおかしいかも アイテムタイプによって0まで消せるか1残すかは判定しないとダメ(武器は手持ち分残したり、消耗品は0までとか)
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

void Inventory::change_weapon() {
    const int size = inventory_[ItemType::Weapon].size();
    for (size_t i = 0; i < size; ++i) {
        // 現在の武器と一致したら
        if (current_weapon_.id == inventory_[ItemType::Weapon][i].id) {
            ItemDataManager& item_data = ItemDataManager::get_instance();

            // 次の武器にする
            int next_index = i + 1 >= size ? 0 : i + 1;
            current_weapon_ = item_data.get_weapon(inventory_[ItemType::Weapon][next_index].id);
            break;
        }
    }
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
            if (i->count <= 0) i = inventory_[type].erase(i);   // 0以下なら枠から消す
            break;  // 同一IDはほかに存在しないので終了
        }
        else {
            ++i;
        }
    }
}
