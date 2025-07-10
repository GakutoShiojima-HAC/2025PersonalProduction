// -----------------------------------------------------------------------------------------
//  File          : FieldActor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/06/25
//  Updated       : 2025/06/25
//  Description   : ゲーム内に登場する、動く地形の親クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef FIELD_ACTOR_H_
#define FIELD_ACTOR_H_

#include "Engine/Core/Actor/Actor.h"

class Ray;
class Line;
class BoundingSphere;

class FieldActor : public Actor {
public:
    virtual void draw() const override;

public:
    /// <summary>
    /// レイとの衝突判定
    /// </summary>
    /// <param name="ray">= レイ</param>
    /// <param name="max_distance">= レイの長さ</param>
    /// <param name="intersect">= 衝突した交点の受取先</param>
    /// <param name="plane">= 衝突したプレーンの受取先</param>
    /// <returns>衝突していたら真</returns>
    virtual bool collide(
        const Ray& ray, 
        float max_distance,
        GSvector3* intersect = nullptr, 
        GSplane* plane = nullptr
    ) const;
    
    /// <summary>
    /// 線分との衝突判定
    /// </summary>
    /// <param name="line">= 線分</param>
    /// <param name="intersect">= 衝突した交点の受取先</param>
    /// <param name="plane">= 衝突したプレーンの受取先</param>
    /// <returns>衝突していたら真</returns>
    virtual bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, 
        GSplane* plane = nullptr
    ) const;

    /// <summary>
    /// 球体との衝突判定
    /// </summary>
    /// <param name="sphere">= 球体</param>
    /// <param name="intersect">= 衝突した交点の受取先</param>
    /// <returns>衝突していたら真</returns>
    virtual bool collide(
        const BoundingSphere& sphere,
        GSvector3* intersect = nullptr
    ) const;

protected:
    // メッシュ
    GSuint mesh_{ 0 };
    // 衝突判定用メッシュ
    GSuint mesh_collider_{ 0 };

};

#endif
