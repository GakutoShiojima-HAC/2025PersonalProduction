#include "Stage/StageFile.h"
#include "Engine/Utils/MyJson.h"

const std::string STAGE_DATA_FOLDER{ "Resource/Private/Stage/" };

StageFile::StageFile() {
    stages_.clear();

    json j;
    if (!MyJson::is_json("Resource/Private/Stage/config.json", j)) return;

    tutorial_ = MyJson::get_string(j, "Tutorial");
    stages_.push_back(MyJson::get_string(j, "Lobby"));

    if (j.contains("Stage") && j["Stage"].is_array()) {
        for (const auto& item : j["Stage"]) {
            stages_.push_back(item.get<std::string>());
        }
    }
}

std::string StageFile::get_path(int stage_id) const {
    if (stage_id < 0) return STAGE_DATA_FOLDER + tutorial_;
    else if (stage_id < (int)stages_.size()) return STAGE_DATA_FOLDER + stages_[stage_id];
    else return "ERROR";
}
