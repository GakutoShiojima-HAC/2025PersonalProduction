// -----------------------------------------------------------------------------------------
//  File        : BoundingSphere.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/07/09
//  Description : ���̏Փ˔���
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
    /// // ���s�ړ�
    /// </summary>
    BoundingSphere translate(const GSvector3& position) const;
    
    /// <summary>
    /// ���W�ϊ�
    /// </summary>
    BoundingSphere transform(const GSmatrix4& matrix) const;

    /// <summary>
    /// ���Ƌ����d�Ȃ��Ă��邩��ԋp
    /// </summary>
    /// <param name="other">= �d�Ȃ��Ă���ΐ^</param>
    bool intersects(const BoundingSphere& other) const;

    /// <summary>
    /// �f�o�b�O�\��
    /// </summary>
    void draw() const;

public:
    // ���a
    float radius{ 0.0f };
    // ���S���W
    GSvector3 center{ 0.0f, 0.0f, 0.0f };

};

#endif
