// -----------------------------------------------------------------------------------------
//  File          : FieldActorManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : �t�B�[���h�A�N�^�[�Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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
    /// �t�B�[���h�A�N�^�[�̍X�V
    /// </summary>
    void update(float delta_time, float time_scale) override;

public:
    /// <summary>
    /// ���C�Ƃ̏Փ˔���
    /// </summary>
    /// <param name="max_distance">= ���C�̒���</param>
    /// <param name="intersect">= ��_�̎���</param>
    /// <param name="plane">= ���ʂ̎���</param>
    /// <returns>�Փ˂��Ă�����^��ԋp</returns>
    FieldActor* collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    
    /// <summary>
    /// �����Ƃ̏Փ˔���
    /// </summary>
    /// <param name="intersect">= ��_�̎���</param>
    /// <param name="plane">= ���ʂ̎���</param>
    /// <returns>�Փ˂��Ă�����^��ԋp</returns>
    FieldActor* collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    
    /// <summary>
    /// ���̂Ƃ̏Փ˔���
    /// </summary>
    /// <param name="intersect">= ��_�̎���</param>
    /// <param name="plane">= ���ʂ̎���</param>
    /// <returns>�Փ˂��Ă�����^��ԋp</returns>
    FieldActor* collide(
        const BoundingSphere& sphere,
        GSvector3* collided_center = nullptr) const;

public:
    // �R�s�[�֎~
    FieldActorManager(const FieldActorManager& other) = delete;
    FieldActorManager& operator = (const FieldActorManager& other) = delete;

};

#endif
