#include "Actor/Gimmick/Elevator/ElevatorGenerator.h"
#include "Actor/Gimmick/Elevator/Elevator.h"

ElevatorGenerator::ElevatorGenerator(const json& j, World* world) {
    world_ = world;

    // json‚Íg‚í‚È‚¢
}

Actor* ElevatorGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    FieldActor* p = new Elevator{ world_, position, MyJson::get_vector3(param, "End"), MyJson::get_float(param, "Time") };
    world_->get_field()->add(p);
    return p;
}
