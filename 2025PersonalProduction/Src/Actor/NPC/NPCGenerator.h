// -----------------------------------------------------------------------------------------
//  File          : NPCGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/3
//  Updated       : 2026/02/3
//  Description   : NPCのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NPC_GENERATOR_H_
#define NPC_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"
#include "Actor/NPC/NPC.h"

static const std::string NPCGeneratorKey{ "NPC" };

class NPCGenerator : public IActorGenerator {
public:
    NPCGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

    NPCData data_;

};

#endif
