#include "Stage/StageData.h"
#include "Engine/Utils/MyJson.h"

static const std::string DATA_PATH{ "/data.json" };

void StageData::load(const std::string& folder_path) {
    data_.folder = folder_path;

    json j;
    if (!MyJson::is_json(folder_path + DATA_PATH, j)) return;

    data_.name = MyJson::get_string(j, "Name");
    data_.use_timer = MyJson::get_boolean(j, "Timer");
    data_.use_result = MyJson::get_boolean(j, "Result");
}

StageDataStruct& StageData::data() {
    return data_;
}

const StageDataStruct& StageData::data() const {
    return data_;
}
