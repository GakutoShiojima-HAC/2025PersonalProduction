// -----------------------------------------------------------------------------------------
//  File          : VikingGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/14
//  Updated       : 2026/01/14
//  Description   : Vikingのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef VIKING_GENERATOR_H_
#define VIKING_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"
#include "Viking.h"

static const std::string VikingGeneratorKey{ "Viking" };

class VikingGenerator : public IActorGenerator {
public:
    VikingGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

    MyEnemyInfo my_info_;

};

#endif
