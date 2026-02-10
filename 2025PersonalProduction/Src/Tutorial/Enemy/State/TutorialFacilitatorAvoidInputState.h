// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitatorAvoidInputState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : 回避入力を覚えてもらうステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIALFACILITATOR_AVOIDINPUT_STATE_H_
#define TUTORIALFACILITATOR_AVOIDINPUT_STATE_H_

#include "TutorialFacilitatorState.h"

class TutorialFacilitatorAvoidInputState : public TutorialFacilitatorState {
public:
	 TutorialFacilitatorAvoidInputState(TutorialFacilitator& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override;

	void exit() override;

private:
    bool is_show_{ false };

    // 解除したかどうか
    bool is_avoid_disable_{ true };
    // 表示タイマー
    float timer_{ 0.0f };
};

#endif
