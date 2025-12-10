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
        save_data_.player_level = MyJson::get_int(j, "PlayerLevel", 1);

        if (j.contains("Score") && j["Score"].is_array()) {
            for (const auto& item : j["Score"]) save_data_.score[item[0].get<int>()] = item[1].get<int>();
        }
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
    data["PlayerLevel"] = save_data_.player_level;
    for (const auto& [key, value] : save_data_.score) data["Score"].push_back({ key, value });

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

int GameSaveData::get_stage_score(int stage_num) const {
    auto it = save_data_.score.find(stage_num);
    if (it != save_data_.score.end()) return it->second;
    return 0;
}

void GameSaveData::set_clear_stage(int stage_num, int score) {
    if (stage_num == -1) {
        save_data_.stage = stage_num + 1;
    }
    else if (stage_num >= 0) {
        if(stage_num > save_data_.stage) save_data_.stage = stage_num;
    }
    else {
        // -1未満は不要
    }

    // スコア保存
    if (stage_num >= -1) {
        auto it = save_data_.score.find(stage_num);
        if (it != save_data_.score.end()) it->second = score > it->second ? score : it->second;
        else save_data_.score[stage_num] = score;
    }
}

int GameSaveData::calc_final_score(int action_score, float time, int respawn_count) const {
    float final_score = (float)action_score + 1000.0f;

    // 復活回数分基礎を減らす
    final_score -= final_score * respawn_count * 0.1f;
    // 時間を引く
    final_score -= time * 1.5f;

    return CLAMP((int)final_score, 0, INT_MAX);
}
