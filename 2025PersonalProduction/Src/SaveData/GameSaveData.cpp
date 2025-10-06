#include "SaveData/GameSaveData.h"
#include "Engine/Utils/Folder.h"

void GameSaveData::load(const std::string& folder_path) {
    save_folder_path_ = "SaveData/" + folder_path;

    inventory_.load(save_folder_path_);
}

void GameSaveData::save() {
    // セーブ先フォルダを作成
    MyLib::create_folder(save_folder_path_);

    inventory_.save(save_folder_path_);
}

void GameSaveData::clear() {
    inventory_.clear();
}

Inventory& GameSaveData::inventory() {
    return inventory_;
}
