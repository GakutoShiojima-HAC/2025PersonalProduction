#include "PlayReact.h"
#include "Engine/Core/Actor/Actor.h"

PlayReact::PlayReact(IWorld* world, const std::string& name) : ICinemaBehavior(world) {
    trigger_name_ = name;
}

void PlayReact::react(Actor& other) {
    if (!is_play_ && other.name() == trigger_name_) is_start_ = true;
}
