// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitatorChaseState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : 追いかけるステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIALFACILITATOR_CHASE_STATE_H_
#define TUTORIALFACILITATOR_CHASE_STATE_H_

#include "TutorialFacilitatorState.h"

class TutorialFacilitatorChaseState : public TutorialFacilitatorState {
public:
    TutorialFacilitatorChaseState(TutorialFacilitator& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override;

};

#endif
