#ifndef I_ACTOR_GENERATER_H_
#define I_ACTOR_GENERATER_H_

#include <gslib.h>

class World;

class IActorGenerator {
public:
    IActorGenerator() = default;

    virtual ~IActorGenerator() = default;

public:
    /// <summary>
    /// ����
    /// </summary>
    /// <param name="position">= ������̍��W</param>
    /// <param name="lookat">= �����_</param>
    /// <param name="hp">= �̗�</param>
    /// <param name="damage">= �U����</param>
    virtual void generate(const GSvector3& position, const GSvector3& lookat, int hp = 1, int damage = 1) = 0;

protected:
    World* world_{ nullptr };

public:
    // �R�s�[�֎~
    IActorGenerator(const IActorGenerator& other) = delete;
    IActorGenerator& operator = (const IActorGenerator& other) = delete;

};

#endif
