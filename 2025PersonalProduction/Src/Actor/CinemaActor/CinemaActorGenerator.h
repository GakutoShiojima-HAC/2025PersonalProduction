#ifndef CINEMA_ACTOR_GENERATOR_H_
#define CINEMA_ACTOR_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string CinemaActorGeneratorKey{ "CinemaActor" };

class CinemaActorGenerator: public IActorGenerator {
public:
    CinemaActorGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

private:
    World* world_{ nullptr };

};

#endif
