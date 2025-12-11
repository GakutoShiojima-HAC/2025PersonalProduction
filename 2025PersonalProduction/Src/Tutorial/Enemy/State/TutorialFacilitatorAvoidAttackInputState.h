// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitatorAvoidAttackInputState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : 回避攻撃入力を覚えてもらうステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIALFACILITATOR_AVOIDATTACKINPUT_STATE_H_
#define TUTORIALFACILITATOR_AVOIDATTACKINPUT_STATE_H_

#include "TutorialFacilitatorState.h"

class TutorialFacilitatorAvoidAttackInputState : public TutorialFacilitatorState {
public:
	 TutorialFacilitatorAvoidAttackInputState(TutorialFacilitator& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

};

#endif
