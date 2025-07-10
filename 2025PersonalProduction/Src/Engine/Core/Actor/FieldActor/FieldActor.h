// -----------------------------------------------------------------------------------------
//  File          : FieldActor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/06/25
//  Updated       : 2025/06/25
//  Description   : �Q�[�����ɓo�ꂷ��A�����n�`�̐e�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
    /// ���C�Ƃ̏Փ˔���
    /// </summary>
    /// <param name="ray">= ���C</param>
    /// <param name="max_distance">= ���C�̒���</param>
    /// <param name="intersect">= �Փ˂�����_�̎���</param>
    /// <param name="plane">= �Փ˂����v���[���̎���</param>
    /// <returns>�Փ˂��Ă�����^</returns>
    virtual bool collide(
        const Ray& ray, 
        float max_distance,
        GSvector3* intersect = nullptr, 
        GSplane* plane = nullptr
    ) const;
    
    /// <summary>
    /// �����Ƃ̏Փ˔���
    /// </summary>
    /// <param name="line">= ����</param>
    /// <param name="intersect">= �Փ˂�����_�̎���</param>
    /// <param name="plane">= �Փ˂����v���[���̎���</param>
    /// <returns>�Փ˂��Ă�����^</returns>
    virtual bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, 
        GSplane* plane = nullptr
    ) const;

    /// <summary>
    /// ���̂Ƃ̏Փ˔���
    /// </summary>
    /// <param name="sphere">= ����</param>
    /// <param name="intersect">= �Փ˂�����_�̎���</param>
    /// <returns>�Փ˂��Ă�����^</returns>
    virtual bool collide(
        const BoundingSphere& sphere,
        GSvector3* intersect = nullptr
    ) const;

protected:
    // ���b�V��
    GSuint mesh_{ 0 };
    // �Փ˔���p���b�V��
    GSuint mesh_collider_{ 0 };

};

#endif
