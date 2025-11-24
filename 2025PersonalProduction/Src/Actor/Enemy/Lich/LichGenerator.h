// -----------------------------------------------------------------------------------------
//  File          : LichGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/21
//  Updated       : 2025/11/21
//  Description   : Lichのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LICH_GENERATOR_H_
#define LICH_GENERATOR_H_

#include "Engine/Core/Actor/IActorGenerator.h"
#include "Lich.h"

static const std::string LichGeneratorKey{ "Lich" };

class LichGenerator : public IActorGenerator {
public:
    LichGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

    LichInfo info_;

    MyEnemyInfo my_info_;

};

#endif
