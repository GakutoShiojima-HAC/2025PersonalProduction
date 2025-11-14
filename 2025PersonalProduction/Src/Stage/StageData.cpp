#include "Stage/StageData.h"
#include "Engine/Utils/MyJson.h"

static const std::string DATA_PATH{ "/data.json" };

StageLoadConfigData StageData::load(const std::string& folder_path) {
    data_.folder = folder_path;
    StageLoadConfigData load_data;

    json j;
    if (!MyJson::is_json(folder_path + DATA_PATH, j)) return load_data;

    // 構成データ読み込み
    data_.name = MyJson::get_string(j, "Name");
    data_.light_angle = MyJson::get_vector3(j, "LightAngle");
    data_.use_timer = MyJson::get_boolean(j, "Timer");
    data_.use_result = MyJson::get_boolean(j, "Result");

    // ロード用構成データ読み込み
    load_data.navmesh = MyJson::get_string(j, "NavMesh");
    load_data.lightmap = MyJson::get_string(j, "Lightmap");
    load_data.refprobe = MyJson::get_string(j, "RefProbe");

    return load_data;
}

StageConfigData& StageData::data() {
    return data_;
}

const StageConfigData& StageData::data() const {
    return data_;
}
