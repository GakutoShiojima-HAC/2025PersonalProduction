#include "Actor/Gimmick/Elevator/ElevatorGenerator.h"
#include "Actor/Gimmick/Elevator/Elevator.h"

ElevatorGenerator::ElevatorGenerator(const json& j, World* world) {
    world_ = world;

    // json‚ÍŽg‚í‚È‚¢
}

void ElevatorGenerator::generate(const GSvector3& position, const GSvector3& lookat, int hp, int damage, const json& param) {
    world_->get_field()->add(new Elevator{ world_, position, MyJson::get_vector3(param, "End"), MyJson::get_float(param, "Time") });
}
