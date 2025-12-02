#include "StaticEffect.h"
#include <gslib.h>
#include "Engine/Utils/MyJson.h"
#include "Assets.h"

void StaticEffect::load(const std::string& json_path) {
    clear();

    json j;
    if (!MyJson::is_json(json_path, j)) return;

    for (const auto& [key, value]  : j.items()) {
        // 再生するエフェクトの種類を取得
        const GSuint id = Assets::to_effect_id(key);
        if (id == (GSuint)EffectID::NONE) continue;
        // 座標を全取得
        for (const auto& row : value) {
            GSvector3 pos;
            pos.x = row[0].get<float>();
            pos.y = row[1].get<float>();
            pos.z = row[2].get<float>();
            positions_[id].push_back(pos);
        }
    }
}

void StaticEffect::generate() {
    for (const auto& type : positions_) {
        const GSuint id = type.first;
        for (const auto& pos : type.second) {
            handles_.push_back(gsPlayEffect(id, &pos));
        }
    }
    positions_.clear();
}

void StaticEffect::clear() {
    for (const auto& handle : handles_) {
        gsStopEffect(handle);
    }
    handles_.clear();
    positions_.clear();
}
