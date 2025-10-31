// -----------------------------------------------------------------------------------------
//  File          : FieldActorManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : フィールドアクター管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef FIELD_ACTOR_MANAGER_H_
#define FIELD_ACTOR_MANAGER_H_

#include "Engine/Core/Actor/ActorManager.h"

using namespace std;

class FieldActor;
class Ray;
class Line;
class BoundingSphere;
struct GSvector3;
struct GSplane;

class FieldActorManager : public ActorManager {
public:
    FieldActorManager() = default;

    ~FieldActorManager();

public:
    /// <summary>
    /// フィールドアクターの更新
    /// </summary>
    void update(float delta_time, float time_scale) override;

public:
    /// <summary>
    /// レイとの衝突判定
    /// </summary>
    /// <param name="max_distance">= レイの長さ</param>
    /// <param name="intersect">= 交点の受取先</param>
    /// <param name="plane">= 平面の受取先</param>
    /// <returns>衝突していたら真を返却</returns>
    FieldActor* collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    
    /// <summary>
    /// 線分との衝突判定
    /// </summary>
    /// <param name="intersect">= 交点の受取先</param>
    /// <param name="plane">= 平面の受取先</param>
    /// <returns>衝突していたら真を返却</returns>
    FieldActor* collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    
    /// <summary>
    /// 球体との衝突判定
    /// </summary>
    /// <param name="intersect">= 交点の受取先</param>
    /// <param name="plane">= 平面の受取先</param>
    /// <returns>衝突していたら真を返却</returns>
    FieldActor* collide(
        const BoundingSphere& sphere,
        GSvector3* collided_center = nullptr) const;

public:
    // コピー禁止
    FieldActorManager(const FieldActorManager& other) = delete;
    FieldActorManager& operator = (const FieldActorManager& other) = delete;

};

#endif
