// -----------------------------------------------------------------------------------------
//  File          : DebugMarker.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/06
//  Updated       : 2025/08/06
//  Description   : デバッグ用描画ラップクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef DEBUG_MARKER_H_
#define DEBUG_MARKER_H_

#include <gslib.h>

using namespace std;

namespace MyLib {
	/// <summary>
	/// 球を描画する
	/// </summary>
	/// <param name="position">= 座標</param>
	/// <param name="radius">= 半径</param>
	/// <param name="color">= 色</param>
	inline void draw_sphere(const GSvector3& position, float radius, const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) {
		// ライティングを無効にする
		glDisable(GL_LIGHTING);
		glColor4f(color.r, color.g, color.b, color.a);

		// 球を描画する
		GLUquadric* quad = gluNewQuadric();
		gluQuadricDrawStyle(quad, GLU_FILL); // ワイヤーフレームにしない
		gluQuadricNormals(quad, GLU_SMOOTH);

		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		gluSphere(quad, radius, 20, 20);
		glPopMatrix();

		gluDeleteQuadric(quad);

		// ライティングを有効にする
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_LIGHTING);
	}

	/// <summary>
	/// 線を描画する
	/// </summary>
	/// <param name="start">= 開始座標</param>
	/// <param name="end">= 終了座標</param>
	/// <param name="color">= 色</param>
	inline void draw_line(const GSvector3& start, const GSvector3& end, const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) {
		// ライティングを無効にする
		glDisable(GL_LIGHTING);
		glColor4f(color.r, color.g, color.b, color.a);

		// 線を描画する
		glBegin(GL_LINES);
		glVertex3f(start.x, start.y, start.z);
		glVertex3f(end.x, end.y, end.z);
		glEnd();

		// ライティングを有効にする
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_LIGHTING);
	}

}

#endif
