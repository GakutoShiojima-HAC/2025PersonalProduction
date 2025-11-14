#ifndef SIMPLE_ENEMY_GENERATOR_H_
#define SIMPLE_ENEMY_GENERATOR_H_

#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string SimpleEnemyGeneratorKey{ "SimpleEnemy" };

class SimpleEnemyGenerator : public IActorGenerator {
public:
    SimpleEnemyGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) override;

private:
    World* world_{ nullptr };

    SimpleEnemyInfo info_;

};

#endif
