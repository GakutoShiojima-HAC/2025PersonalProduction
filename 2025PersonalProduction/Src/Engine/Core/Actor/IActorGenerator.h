#ifndef I_ACTOR_GENERATER_H_
#define I_ACTOR_GENERATER_H_

#include <gslib.h>
#include "Engine/Utils/MyJson.h"

class World;

class IActorGenerator {
public:
    IActorGenerator() = default;

    virtual ~IActorGenerator() = default;

public:
    /// <summary>
    /// 生成
    /// </summary>
    /// <param name="position">= 生成先の座標</param>
    /// <param name="lookat">= 注視点</param>
    /// <param name="hp">= 体力</param>
    /// <param name="damage">= 攻撃力</param>
    virtual void generate(const GSvector3& position, const GSvector3& lookat, int hp = 1, int damage = 1, const json& param = json{}) = 0;

protected:
    World* world_{ nullptr };

public:
    // コピー禁止
    IActorGenerator(const IActorGenerator& other) = delete;
    IActorGenerator& operator = (const IActorGenerator& other) = delete;

};

#endif
