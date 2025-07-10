// -----------------------------------------------------------------------------------------
//  File        : Ray.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/26
//  Description : レイ管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef RAY_H_
#define RAY_H_

#include <GSvector3.h>

class Ray {
public:
    Ray() = default;

    Ray(const GSvector3& position, const GSvector3& direction) :
        position{ position }, direction{ direction } {
    }

public:
    /// <summary>
    /// レイの描画
    /// </summary>
    void draw(float max_distance, float r = 0.0f, float g = 1.0f, float b = 0.0f) const {
        // ライティングを無効にする
        glDisable(GL_LIGHTING);
        glColor3f(r, g, b);
        glBegin(GL_LINES);
        glVertex3f(position.x, position.y, position.z);
        const GSvector3 end = position + direction.normalized() * max_distance;
        glVertex3f(end.x, end.y, end.z);
        glEnd();
        // 白に戻す
        glColor3f(1.0f, 1.0f, 1.0f);
        // ライティングを有効にする
        glEnable(GL_LIGHTING);
    }

public:
    // 座標
    GSvector3 position{ 0.0f, 0.0f, 0.0f };
    // 方向
    GSvector3 direction{ 0.0f, 0.0f, 0.0f };

};

#endif
