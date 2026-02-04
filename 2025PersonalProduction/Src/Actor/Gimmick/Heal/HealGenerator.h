// -----------------------------------------------------------------------------------------
//  File          : HealGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/3
//  Updated       : 2026/02/3
//  Description   : Healのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef HEAL_GENERATOR_H_
#define HEAL_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string HealGeneratorKey{ "Heal" };

class HealGenerator : public IActorGenerator {
public:
    HealGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

};

#endif
