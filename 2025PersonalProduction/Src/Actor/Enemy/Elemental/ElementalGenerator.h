// -----------------------------------------------------------------------------------------
//  File          : ElementalGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/01/20
//  Updated       : 2026/01/20
//  Description   : エレメンタルのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ELEMENTAL_GENERATOR_H_
#define ELEMENTAL_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"
#include "Elemental.h"

static const std::string ElementalGeneratorKey{ "Elemental" };

class ElementalGenerator : public IActorGenerator {
public:
    ElementalGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

    ElementalInfo info_;

    MyEnemyInfo my_info_;

};

#endif
