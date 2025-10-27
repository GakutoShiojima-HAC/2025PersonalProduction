// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemyFindState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/24
//  Updated       : 2025/10/24
//  Description   : シンプルエネミーの敵発見状態
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_FIND_STATE_H_
#define SIMPLE_ENEMY_FIND_STATE_H_

#include "State/SimpleEnemy/SimpleEnemyState.h"

class SimpleEnemyFindState : public SimpleEnemyState {
public:
    SimpleEnemyFindState(SimpleEnemy& owner);

public:
    void enter() override {};

    void update(float delta_time) override;

    void draw() const override {};

    void draw_gui() const override {};

    void exit() override {};

};

#endif
