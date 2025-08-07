// -----------------------------------------------------------------------------------------
//  File          : DebugMarker.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/06
//  Updated       : 2025/08/06
//  Description   : �f�o�b�O�p�`�惉�b�v�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef DEBUG_MARKER_H_
#define DEBUG_MARKER_H_

#include <gslib.h>

using namespace std;

namespace MyLib {
	/// <summary>
	/// ����`�悷��
	/// </summary>
	/// <param name="position">= ���W</param>
	/// <param name="radius">= ���a</param>
	/// <param name="color">= �F</param>
	inline void draw_sphere(const GSvector3& position, float radius, const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) {
		// ���C�e�B���O�𖳌��ɂ���
		glDisable(GL_LIGHTING);
		glColor4f(color.r, color.g, color.b, color.a);

		// ����`�悷��
		GLUquadric* quad = gluNewQuadric();
		gluQuadricDrawStyle(quad, GLU_FILL); // ���C���[�t���[���ɂ��Ȃ�
		gluQuadricNormals(quad, GLU_SMOOTH);

		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		gluSphere(quad, radius, 20, 20);
		glPopMatrix();

		gluDeleteQuadric(quad);

		// ���C�e�B���O��L���ɂ���
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_LIGHTING);
	}

	/// <summary>
	/// ����`�悷��
	/// </summary>
	/// <param name="start">= �J�n���W</param>
	/// <param name="end">= �I�����W</param>
	/// <param name="color">= �F</param>
	inline void draw_line(const GSvector3& start, const GSvector3& end, const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) {
		// ���C�e�B���O�𖳌��ɂ���
		glDisable(GL_LIGHTING);
		glColor4f(color.r, color.g, color.b, color.a);

		// ����`�悷��
		glBegin(GL_LINES);
		glVertex3f(start.x, start.y, start.z);
		glVertex3f(end.x, end.y, end.z);
		glEnd();

		// ���C�e�B���O��L���ɂ���
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_LIGHTING);
	}

}

#endif
