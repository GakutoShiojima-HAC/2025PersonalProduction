// -----------------------------------------------------------------------------------------
//  File          : TreasureChestGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/12
//  Updated       : 2025/12/12
//  Description   : TreasureChestのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TREASURECHEST_GENERATOR_H_
#define TREASURECHEST_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string TreasureChestGeneratorKey{ "TreasureChest" };

class TreasureChestGenerator : public IActorGenerator {
public:
    TreasureChestGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

};

#endif
