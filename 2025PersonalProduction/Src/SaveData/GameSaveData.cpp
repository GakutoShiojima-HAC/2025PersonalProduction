#include "SaveData/GameSaveData.h"
#include "Engine/Utils/Folder.h"
#include "Engine/Utils/MyJson.h"

const std::string SAVE_FOLDER_PATH{ "SaveData/" };

void GameSaveData::load(const std::string& file_path) {
    // 初期化
    save_file_path_ = "SaveData/test.json"; // TODO file_pathを使う get_all_save_file()を使ってから
    clear();

    json j;
    if (!MyJson::is_json(save_file_path_, j)) return;

    // セーブデータの読み込み
    {
        save_data_.stage = MyJson::get_int(j, "Stage");
    }
    // インベントリの読み込み
    {
        inventory_.load(j["Inventory"]);
    }
}

void GameSaveData::save() {
    // セーブ先フォルダを作成
    MyLib::create_folder(SAVE_FOLDER_PATH);

    // データを保存
    ordered_json data;
    data["Stage"] = save_data_.stage;
    data["Inventory"] = inventory_.save_object();

    MyLib::write_to_file(save_file_path_, data);
}

void GameSaveData::clear() {
    save_data_ = SaveData{};
    inventory_.clear();
}

Inventory& GameSaveData::inventory() {
    return inventory_;
}

const SaveData& GameSaveData::get() const {
    return save_data_;
}

std::vector<string> GameSaveData::get_all_save_file() const {
    return MyLib::get_all_file_path(SAVE_FOLDER_PATH, "json");
}

void GameSaveData::set_clear_stage(int stage_num) {
    if (stage_num == -1) {
        save_data_.stage = stage_num + 1;
    }
    else if (stage_num >= 0) {
        if(stage_num > save_data_.stage) save_data_.stage = stage_num;
    }
    else {
        //
    }
}
