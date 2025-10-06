#include "Item/ItemDataManager.h"

const std::string FOLDER_PATH{ "Resource/Private/Item" };

ItemDataManager& ItemDataManager::get_instance() {
    // static変数のインスタンスは１つ
    // インスタンス化も１回のみ
    static ItemDataManager self;
    return self;
}

void ItemDataManager::load() {
    weapon_data_.load(FOLDER_PATH);
}

std::string ItemDataManager::get_name(ItemData::Data data) const {
    switch (data.type) {
    case ItemType::Weapon: return get_weapon(data.id).name;
    default: return "UNDEFINED_ITEM";
    }
    return "UNDEFINED_ITEM";
}

bool ItemDataManager::is_empty(ItemData::Data data) const {
    switch (data.type) {
    case ItemType::Weapon: return get_weapon(data.id).is_empty();
    default: return false;
    }
    return false;
}

WeaponData::Data ItemDataManager::get_weapon(int id) const {
    return weapon_data_.get(id);
}

int ItemDataManager::weapon_version() const {
    return weapon_data_.version();
}
