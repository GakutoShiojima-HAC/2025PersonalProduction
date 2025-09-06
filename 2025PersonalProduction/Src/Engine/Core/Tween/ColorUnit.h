#ifndef COLOR_UNIT_H_
#define COLOR_UNIT_H_

#include "Engine/Core/Tween/TweenUnit.h"

// Color�^�𓮂������߂�TweenUnit
class ColorUnit : public TweenUnit {
public:
    // �R���X�g���N�^
    ColorUnit(const GScolor& from, const GScolor& to, float duration, std::function<void(const GScolor&)> update_func);
    // �X�V���̏���
    virtual void on_update(float progress) override;

private:
    GScolor from_;                                // �J�n�l
    GScolor to_;                                  // �I���l
    std::function<void(GScolor)> update_func_;    // �X�V���̏���
};

#endif
