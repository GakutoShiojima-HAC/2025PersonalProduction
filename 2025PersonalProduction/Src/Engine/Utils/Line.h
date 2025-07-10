// -----------------------------------------------------------------------------------------
//  File        : Line.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/26
//  Description : �����Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
    /// �����̕`��
    /// </summary>
    void draw(float r = 0.0f, float g = 1.0f, float b = 0.0f) const {
        // ���C�e�B���O�𖳌��ɂ���
        glDisable(GL_LIGHTING);
        glColor3f(r, g, b);
        glBegin(GL_LINES);
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
        glEnd();
        // ���ɖ߂�
        glColor3f(1.0f, 1.0f, 1.0f);
        // ���C�e�B���O��L���ɂ���
        glEnable(GL_LIGHTING);
    }

public:
    // �n�_
    GSvector3 start{ 0.0f, 0.0f, 0.0f };
    // �I�_
    GSvector3 end{ 0.0f, 0.0f, 0.0f };

};

#endif
