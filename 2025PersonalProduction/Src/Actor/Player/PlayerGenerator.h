#ifndef PLAYER_GENERATOR_GENERATOR_H_
#define PLAYER_GENERATOR_GENERATOR_H_

#include "Actor/Player/Player.h"
#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string PlayerGeneratorKey{ "Player" };

class PlayerGenerator : public IActorGenerator {
public:
    PlayerGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

    PlayerInfo info_;

};

#endif
