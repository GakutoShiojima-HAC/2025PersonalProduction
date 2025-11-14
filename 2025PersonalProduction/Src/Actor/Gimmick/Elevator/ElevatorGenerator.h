#ifndef ELEVATOR_GENERATOR_H_
#define ELEVATOR_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string ElevatorGeneratorKey{ "Elevator" };

class ElevatorGenerator : public IActorGenerator {
public:
    ElevatorGenerator(const json& j, World* world);

public:
    void generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) override;

private:
    World* world_{ nullptr };

};

#endif
