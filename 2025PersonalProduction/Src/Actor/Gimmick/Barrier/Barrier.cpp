#include "Actor/Gimmick/Barrier/Barrier.h"
#include "Assets.h"
#include "GameConfig.h"
#include <GSeffect.h>

Barrier::Barrier(IWorld* world, const GSvector3& position, const std::string& break_type, float break_time, float parameter) {
    world_ = world;
    tag_ = ActorTag::Object;
    name_ = "Barrier";
    transform_.position(position);
    origin_position_ = position;

    enable_collider_ = false;
    enable_timescale_ = false;

    mesh_ = (GSuint)MeshID::GimmickBarrier;
    mesh_collider_ = (GSuint)MeshID::GimmickBarrier;

    break_time_ = break_time;
    timer_ = break_time;
    parameter_ = parameter;

    if (break_type == "Down") break_type_ = BarrierBreakType::Down;
    else if (break_type == "Fade") break_type_ = BarrierBreakType::Fade;
}

void Barrier::update(float delta_time) {
    if (is_break_) {
        timer_ -= delta_time / cFPS;

        if (timer_ <= 0.0f) {
            switch (break_type_) {
            case Barrier::BarrierBreakType::Down:
                gsPlayEffect((GSuint)EffectID::DustLarge, &origin_position_);
                break;
            case Barrier::BarrierBreakType::Fade:

                break;
            default:

                break;
            }

            die();
            return;
        }

        // Œo‰ß‰‰o
        switch (break_type_) {
        case Barrier::BarrierBreakType::Down:
            transform_.translate(GSvector3{ 0.0f, -parameter_ * delta_time, 0.0f });
            break;
        case Barrier::BarrierBreakType::Fade:
            break;
        default:
            break;
        }
    }
}

void Barrier::draw() const {
    const float alpha = is_break_ ? timer_ / break_time_ : 1.0f;

    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    gsDrawMesh(mesh_);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // color reset
    glPopMatrix();
}

void Barrier::message(const std::string& message, std::any& param) {
    if (message == "ByTimeline") {
        is_break_ = true;

        switch (break_type_) {
        case Barrier::BarrierBreakType::Down:
            gsPlayEffect((GSuint)EffectID::DustLarge, &origin_position_);
            break;
        case Barrier::BarrierBreakType::Fade:
            break;
        default:
            break;
        }
    }
}
