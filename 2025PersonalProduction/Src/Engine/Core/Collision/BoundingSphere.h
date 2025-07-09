// -----------------------------------------------------------------------------------------
//  File        : BoundingSphere.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/07/09
//  Description : 球体衝突判定
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include <gslib.h>

class BoundingSphere {
public:
    BoundingSphere(float radius = 0.0f, const GSvector3& center = GSvector3{ 0.0f, 0.0f, 0.0f });

    ~BoundingSphere() = default;

public:
    /// <summary>
    /// // 平行移動
    /// </summary>
    BoundingSphere translate(const GSvector3& position) const;
    
    /// <summary>
    /// 座標変換
    /// </summary>
    BoundingSphere transform(const GSmatrix4& matrix) const;

    /// <summary>
    /// 球と球が重なっているかを返却
    /// </summary>
    /// <param name="other">= 重なっていれば真</param>
    bool intersects(const BoundingSphere& other) const;

    /// <summary>
    /// デバッグ表示
    /// </summary>
    void draw() const;

public:
    // 半径
    float radius{ 0.0f };
    // 中心座標
    GSvector3 center{ 0.0f, 0.0f, 0.0f };

};

#endif
