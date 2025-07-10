// -----------------------------------------------------------------------------------------
//  File        : Ray.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/26
//  Description : ���C�Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
    /// ���C�̕`��
    /// </summary>
    void draw(float max_distance, float r = 0.0f, float g = 1.0f, float b = 0.0f) const {
        // ���C�e�B���O�𖳌��ɂ���
        glDisable(GL_LIGHTING);
        glColor3f(r, g, b);
        glBegin(GL_LINES);
        glVertex3f(position.x, position.y, position.z);
        const GSvector3 end = position + direction.normalized() * max_distance;
        glVertex3f(end.x, end.y, end.z);
        glEnd();
        // ���ɖ߂�
        glColor3f(1.0f, 1.0f, 1.0f);
        // ���C�e�B���O��L���ɂ���
        glEnable(GL_LIGHTING);
    }

public:
    // ���W
    GSvector3 position{ 0.0f, 0.0f, 0.0f };
    // ����
    GSvector3 direction{ 0.0f, 0.0f, 0.0f };

};

#endif
