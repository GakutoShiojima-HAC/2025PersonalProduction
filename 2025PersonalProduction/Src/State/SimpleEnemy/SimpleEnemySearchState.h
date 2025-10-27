// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemySearchState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/24
//  Updated       : 2025/10/24
//  Description   : シンプルエネミーの敵検索待機状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_SEARCH_STATE_H_
#define SIMPLE_ENEMY_SEARCH_STATE_H_

#include "State/SimpleEnemy/SimpleEnemyState.h"

class SimpleEnemySearchState : public SimpleEnemyState {
public:
    SimpleEnemySearchState(SimpleEnemy& owner);

public:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

private:
    bool to_find_state_loop_{ false };

    GSuint to_find_state_motion_{ 0 };

    float move_timer_{ 0.0f };

    GSvector3 rotate_foward_{ 0.0f, 0.0f, 0.0f };

};

#endif
