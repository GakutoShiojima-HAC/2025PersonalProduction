#ifndef COLOR_UNIT_H_
#define COLOR_UNIT_H_

#include "Engine/Core/Tween/TweenUnit.h"

// Color型を動かすためのTweenUnit
class ColorUnit : public TweenUnit {
public:
    // コンストラクタ
    ColorUnit(const GScolor& from, const GScolor& to, float duration, std::function<void(const GScolor&)> update_func);
    // 更新時の処理
    virtual void on_update(float progress) override;

private:
    GScolor from_;                                // 開始値
    GScolor to_;                                  // 終了値
    std::function<void(GScolor)> update_func_;    // 更新時の処理
};

#endif
