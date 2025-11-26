#include "Actor/Gimmick/Barrier/Barrier.h"
#include "Assets.h"
#include "GameConfig.h"

Barrier::Barrier(IWorld* world, const GSvector3& position, const std::string& break_type, float break_time, float parameter) {
    world_ = world;
    transform_.position(position);
    tag_ = ActorTag::Object;
    name_ = "Barrier";
    enable_collider_ = false;
    enable_timescale_ = false;
    mesh_ = (GSuint)MeshID::GimmickBarrier;
    mesh_collider_ = (GSuint)MeshID::GimmickBarrier;

    timer_ = break_time;
    parameter_ = parameter;

    if (break_type == "Down") break_type_ = BarrierBreakType::Down;
}

void Barrier::update(float delta_time) {
    if (is_break_) {
        timer_ -= delta_time / cFPS;

        if (timer_ <= 0.0f) {
            die();
            return;
        }

        // ”j‰ó‰‰o
        if (break_type_ == BarrierBreakType::Down) {
            transform_.translate(GSvector3{ 0.0f, -parameter_ * delta_time, 0.0f });
        }
    }
}

void Barrier::message(const std::string& message, std::any& param) {
    if (message == "ByTimeline") is_break_ = true;
}
