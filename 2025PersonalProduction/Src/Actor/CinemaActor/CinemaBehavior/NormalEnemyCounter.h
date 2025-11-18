// -----------------------------------------------------------------------------------------
//  File          : NormalEnemyCounter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/18
//  Updated       : 2025/11/18
//  Description   : 通常エネミーをカウントする
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#include "ICinemaBehavior.h"

class NormalEnemyCounter : public ICinemaBehavior {
public:
    NormalEnemyCounter(IWorld* world, int enemy_count);

public:
    void update(float delta_time) override;

    void draw_gui() const override;

private:
    // 通常エネミー以外のエネミーの数
    int counter_{ 0 };

};
