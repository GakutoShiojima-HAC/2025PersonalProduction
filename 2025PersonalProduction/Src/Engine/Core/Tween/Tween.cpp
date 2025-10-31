#include "Engine/Core/Tween/Tween.h"
#include "Engine/Core/Tween/ValueUnit.h"
#include "Engine/Core/Tween/Vector2Unit.h"
#include "Engine/Core/Tween/Vector3Unit.h"
#include "Engine/Core/Tween/ColorUnit.h"

std::list<TweenUnit*> Tween::units_;

void Tween::add(TweenUnit* unit) {
    units_.push_back(unit);
}

void Tween::update(float delta_time) {
    // 全てのTweenUnitの更新処理を呼び出す
    for (TweenUnit* unit : units_) {
        if (unit->is_finished()) continue;
        unit->update(delta_time);
    }

    // 終了したTweenUnitは削除する
    for (auto itr = units_.begin(); itr != units_.end(); ) {
        if ((*itr)->is_finished()) {
            // 中身のTweenUnitをdeleteしてから
            delete* itr;
            // リストの要素を削除する
            itr = units_.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

void Tween::update(float delta_time, float scale_time) {
    // 全てのTweenUnitの更新処理を呼び出す
    for (TweenUnit* unit : units_) {
        if (unit->is_finished()) continue;
        unit->update(unit->is_enable_timescale() ? scale_time : delta_time);
    }

    // 終了したTweenUnitは削除する
    for (auto itr = units_.begin(); itr != units_.end(); ) {
        if ((*itr)->is_finished()) {
            // 中身のTweenUnitをdeleteしてから
            delete* itr;
            // リストの要素を削除する
            itr = units_.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

void Tween::clear() {
    for (TweenUnit* tween : units_) {
        delete tween;
    }
    units_.clear();
}

TweenUnit& Tween::value(float from, float to, float duration, std::function<void(float)> update_func) {
    TweenUnit* unit = new ValueUnit(from, to, duration, update_func);
    add(unit); // リストに加えて管理対象とする
    return *unit; // TweenUnitの参照を返却する
}

TweenUnit& Tween::vector2(const GSvector2& from, const GSvector2& to, float duration, std::function<void(const GSvector2&)> update_func) {
    TweenUnit* unit = new Vector2Unit(from, to, duration, update_func);
    add(unit);
    return *unit;
}

TweenUnit& Tween::vector3(const GSvector3& from, const GSvector3& to, float duration, std::function<void(const GSvector3&)> update_func) {
    TweenUnit* unit = new Vector3Unit(from, to, duration, update_func);
    add(unit);
    return *unit;
}

TweenUnit& Tween::color(const GScolor& from, const GScolor& to, float duration, std::function<void(const GScolor&)> update_func) {
    TweenUnit* unit = new ColorUnit(from, to, duration, update_func);
    add(unit);
    return *unit;
}

TweenUnit& Tween::delay_call(float delay_time, std::function<void()> callback) {
    TweenUnit* unit = new TweenUnit(delay_time);
    unit->on_complete(callback);
    add(unit);
    return *unit;
}

void Tween::cancel(const std::string& name) {
    // 全ての項目を走査して、名前が一致したものをキャンセル対象にする
    for (const auto& unit : units_) {
        if (unit->name() == name) unit->cancel();
    }
}
