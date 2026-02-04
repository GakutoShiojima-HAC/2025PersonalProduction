#include "NPC.h"

#include "State/NPC/NPCState.h"
#include "State/NPC/NPCActionState.h"
#include "State/Actor/Character/CharacterIdleState.h"
#include "Actor/Player/Player.h"

NPC::NPC(IWorld* world, const GSvector3& position, const GSvector3& rotate, const NPCData& data) :
    data_{ data } {
    world_ = world;
    tag_ = ActorTag::NPC;
    name_ = data.name;

    init_parameter(PawnParameter::get_type(data.pawn_type));
    mesh_ = { data.skinmesh, data.skinmesh, data.skinmesh };

    is_interact_ = false;

    transform_.position(position);
    transform_.eulerAngles(rotate);
    collide_field();
    mesh_.transform(transform_.localToWorldMatrix());

    origin_rotate_ = transform_.eulerAngles();

    add_state();
    use_force_external_ = false;
    change_state((GSuint)NPCStateType::Idle, data.motion_idle, true);
}

void NPC::update(float delta_time) {
    update_state(delta_time);
    update_physics(delta_time);
    collide_field();
    update_mesh(delta_time);
}

void NPC::draw() const {
    mesh_.draw();
}

void NPC::message(const std::string& message, std::any& param) {
    if (message == "PlayerInteract" && param.type() == typeid(Player*) && !is_interact_) {
        Player* player = std::any_cast<Player*>(param);
        if (player != nullptr && !world_->is_playing_timeline()) {
            is_interact_ = true;
            world_->stop_timeline();
            world_->play_timeline(data_.timeline);
            change_state((GSuint)NPCStateType::Action, data_.motion_action, true);
        }
    }
}

bool NPC::can_interact() const {
    return !is_interact_;
}

std::string NPC::interact_text() const {
    return "‰ï˜b";
}

bool NPC::is_action_end() const {
    return !world_->is_playing_timeline();
}

void NPC::end_action() {
    is_interact_ = false;
}

const NPCData& NPC::data() const {
    return data_;
}

void NPC::add_state() {
    state_.add_state((GSuint)NPCStateType::Idle, make_shared<CharacterIdleState>());
    state_.add_state((GSuint)NPCStateType::Action, make_shared<NPCActionState>(*this));
}
