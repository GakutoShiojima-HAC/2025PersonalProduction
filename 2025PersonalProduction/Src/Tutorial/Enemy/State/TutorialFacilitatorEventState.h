// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitatorEventState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/10
//  Updated       : 2025/12/10
//  Description   : イベント中入力を待つステート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIALFACILITATOR_EVENT_STATE_H_
#define TUTORIALFACILITATOR_EVENT_STATE_H_

#include "TutorialFacilitatorState.h"
#include "Engine/Core/Input/Input.h"

class TutorialFacilitatorEventState : public TutorialFacilitatorState {
public:
	 TutorialFacilitatorEventState(TutorialFacilitator& owner);

private:
	void enter() override;

	void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

	void exit() override;

private:
    Input& input_ = Input::get_instance();

    unsigned int player_prev_state_{ 0 };

    float timer_{ 0.0f };

};

#endif
