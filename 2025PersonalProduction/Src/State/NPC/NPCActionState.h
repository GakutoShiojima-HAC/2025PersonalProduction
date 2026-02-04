// -----------------------------------------------------------------------------------------
//  File          : NPCAttackState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/02/03
//  Updated       : 2025/02/03
//  Description   : NPCのアクション状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NPC_ACTION_STATE_H_
#define NPC_ACTION_STATE_H_

#include "State/NPC/NPCState.h"

class NPCActionState : public NPCState {
public:
    NPCActionState(NPC& owner);

private:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

};

#endif
