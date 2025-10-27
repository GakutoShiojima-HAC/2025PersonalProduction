#include "Stage/StageData.h"
#include "Engine/Utils/MyJson.h"

static const std::string DATA_PATH{ "/data.json" };

void StageData::load(const std::string& folder_path) {
    folder_ = folder_path;

    json j;
    if (!MyJson::is_json(folder_path + DATA_PATH, j)) return;

    name_ = MyJson::get_string(j, "Name");
}

std::string StageData::name() const {
    return name_;
}

std::string StageData::folder() const {
    return folder_;
}
