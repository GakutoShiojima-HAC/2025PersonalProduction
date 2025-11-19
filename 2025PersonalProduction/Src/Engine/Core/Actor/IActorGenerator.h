#ifndef I_ACTOR_GENERATER_H_
#define I_ACTOR_GENERATER_H_

#include <gslib.h>
#include "Engine/Utils/MyJson.h"

class World;
class Actor;

class IActorGenerator {
public:
    IActorGenerator() = default;

    virtual ~IActorGenerator() = default;

public:
    /// <summary>
    /// 生成
    /// </summary>
    /// <param name="position">= 生成先の座標</param>
    /// <param name="rotate">= 回転</param>
    /// <param name="hp">= 体力</param>
    /// <param name="damage">= 攻撃力</param>
    /// <returns>生成したアクターのポインタ</returns>
    virtual Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp = 1, int damage = 1, const json& param = json{}) = 0;

    /// <summary>
    /// リスポーンがあり得るかどうか
    /// </summary>
    /// <returns>するなら真を返却</returns>
    virtual bool is_respawn() const { return false; }

protected:
    World* world_{ nullptr };

public:
    // コピー禁止
    IActorGenerator(const IActorGenerator& other) = delete;
    IActorGenerator& operator = (const IActorGenerator& other) = delete;

};

#endif
