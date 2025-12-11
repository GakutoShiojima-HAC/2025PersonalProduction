// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitatorGenerator.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : TutorialFacilitatorのジェネレーター
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIALFACILITATOR_GENERATOR_H_
#define TUTORIALFACILITATOR_GENERATOR_H_

#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Engine/Core/Actor/IActorGenerator.h"

static const std::string TutorialFacilitatorGeneratorKey{ "TutorialFacilitator" };

class TutorialFacilitatorGenerator : public IActorGenerator {
public:
    TutorialFacilitatorGenerator(const json& j, World* world);

public:
    Actor* generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) override;

    bool is_respawn() const override;

private:
    World* world_{ nullptr };

    MyEnemyInfo my_info_;

};

#endif
