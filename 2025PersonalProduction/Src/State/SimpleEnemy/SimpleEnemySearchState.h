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
    void enter() override;

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

private:
    bool can_move_random();

    bool can_move_origin();

    float get_move_time() const;

private:
    enum class MoveState {
        ToRandom,
        ToOrigin,
    };

private:
    bool idle_motion_loop_{ true };
    bool move_motion_loop_{ true };
    GSuint idle_motion_{ 0 };
    GSuint move_motion_{ 0 };

    float move_timer_{ 0.0f };

    MoveState state_{ MoveState::ToRandom };

    GSvector3 rotate_foward_{ 0.0f, 0.0f, 0.0f };

};

#endif
