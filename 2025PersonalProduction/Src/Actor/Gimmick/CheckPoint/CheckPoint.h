// -----------------------------------------------------------------------------------------
//  File          : CheckPoint.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/18
//  Updated       : 2025/11/18
//  Description   : チェックポイントクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CHECK_POINT_H_
#define CHECK_POINT_H_

#include "Engine/Core/Actor/Actor.h"

class CheckPoint : public Actor {
public:
    CheckPoint(IWorld* world, const GSvector3& position, const GSvector3& rotate, float radius);

    ~CheckPoint() = default;

public:
    void react(Actor& other) override;

    void die() override;

};


#endif
