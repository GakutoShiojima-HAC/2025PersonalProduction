#include "Heal.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include "Actor/Player/Player.h"
#include "Engine/Sound/SE.h" 

Heal::Heal(IWorld* world, const GSvector3& position, const GSvector3& rotate, GSuint mesh, int heal_value) {
    world_ = world;
    tag_ = ActorTag::Object;
    name_ = "Heal";

    transform_.position(position);
    transform_.eulerAngles(rotate);

    enable_collider_ = true;
    collider_ = BoundingSphere{ 0.125f };

    mesh_ = mesh;
    heal_value_ = heal_value;
}

void Heal::update(float delta_time) {
    if (is_open_) die();
}

void Heal::draw() const {
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(mesh_);
    glPopMatrix();
}

void Heal::message(const std::string& message, std::any& param) {
    if (message == "PlayerInteract" && param.type() == typeid(Player*) && !is_dead_) {
        is_open_ = true;
        Player* player = std::any_cast<Player*>(param);
        if (player != nullptr) {
            player->hp() = CLAMP(player->hp() + heal_value_, 0, player->max_hp());
            SE::play((GSuint)SEID::Heal);
            GSvector3 position = player->transform().position();
            gsPlayEffect((GSuint)EffectID::Heal, &position);
        }
        return;
    }
}

bool Heal::can_interact() const {
    return !is_open_;
}

std::string Heal::interact_text() const {
    return "‰ñ•œ ( " + std::to_string(heal_value_) + " )";
}
