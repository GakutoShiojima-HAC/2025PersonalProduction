#ifndef BARRIER_GENERATOR_H_
#define BARRIER_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string BarrierGeneratorKey{ "Barrier" };

class BarrierGenerator : public IActorGenerator {
public:
    BarrierGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

private:
    World* world_{ nullptr };

    bool can_generate_{ true }; // 障壁はステージに一つのみとする
};

#endif
