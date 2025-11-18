#include "Actor/Gimmick/Barrier/BarrierGenerator.h"
#include "Actor/Gimmick/Barrier/Barrier.h"

BarrierGenerator::BarrierGenerator(const json& j, World* world) {
    world_ = world;

    // json‚Íg‚í‚È‚¢
}

Actor* BarrierGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    // ¶¬‚Íˆê‘Ì‚Ì‚İ
    if (!can_generate_) return nullptr;

    FieldActor* p = new Barrier(world_, position, MyJson::get_string(param, "Type"), MyJson::get_float(param, "Time"), MyJson::get_float(param, "Value"));
    world_->get_field()->add(p);
    return p;
}
