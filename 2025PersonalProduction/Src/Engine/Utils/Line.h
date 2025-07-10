// -----------------------------------------------------------------------------------------
//  File        : Line.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/26
//  Description : 線分管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LINE_H_
#define LINE_H_

#include <GSvector3.h>

class Line {
public:
    Line() = default;
    
    Line(const GSvector3& start, const GSvector3& end) :
        start{ start }, end{ end } {
    }

public:
    /// <summary>
    /// 線分の描画
    /// </summary>
    void draw(float r = 0.0f, float g = 1.0f, float b = 0.0f) const {
        // ライティングを無効にする
        glDisable(GL_LIGHTING);
        glColor3f(r, g, b);
        glBegin(GL_LINES);
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
        glEnd();
        // 白に戻す
        glColor3f(1.0f, 1.0f, 1.0f);
        // ライティングを有効にする
        glEnable(GL_LIGHTING);
    }

public:
    // 始点
    GSvector3 start{ 0.0f, 0.0f, 0.0f };
    // 終点
    GSvector3 end{ 0.0f, 0.0f, 0.0f };

};

#endif
