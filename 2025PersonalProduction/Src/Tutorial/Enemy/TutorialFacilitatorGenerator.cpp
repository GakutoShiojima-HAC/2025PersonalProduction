#include "TutorialFacilitatorGenerator.h"
#include "Engine/Core/World/World.h"
#include "Actor/Enemy/GetMyEnemyInfo.h"

TutorialFacilitatorGenerator::TutorialFacilitatorGenerator(const json& j, World* world) {
	world_ = world;

    GetMyEnemyInfo::get(my_info_, j);
}

Actor* TutorialFacilitatorGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    MyEnemyInfo my_info = my_info_;
    my_info.hp = hp;
    my_info.damage = damage;

    TutorialFacilitator* p = new TutorialFacilitator{ world_, position, rotate, my_info };

    world_->add_character(p);
    return p;
}

bool TutorialFacilitatorGenerator::is_respawn() const {
    return false;
}
