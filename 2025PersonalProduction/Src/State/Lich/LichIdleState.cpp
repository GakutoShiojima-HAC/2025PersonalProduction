#include "State/Lich/LichIdleState.h"
#include "Actor/Enemy/Lich/Lich.h"
#include "GameConfig.h"
#include "Engine/Utils/MyRandom.h"

LichIdleState::LichIdleState(Lich& owner) :
	LichState(owner) {
    next_state_timer_ = 2.0f;
}

void LichIdleState::enter() {
    
}

void LichIdleState::update(float delta_time) {
    // õ“G”ÍˆÍ“à‚É“G‚ª‚¢‚È‚¢‚È‚ç‘Ò‹@
    if (!owner_.search_target()) {
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // õ“G”ÍˆÍŠO‚É‚¢‚Á‚½‚ç’Ç‚í‚È‚¢
    if (to_target_length > owner_.my_info().search_length * 1.5f) {
        owner_.release_target();
        return;
    }

    // í‚É’Ç]
    owner_.update_look_target(delta_time);

    // ƒ^ƒCƒ}[‚ği‚ß‚é
    if (next_state_timer_ > 0.0f) {
        next_state_timer_ -= delta_time / cFPS;
        return;
    }

    // Ú‹ßUŒ‚‹——£‚É“G‚ª‘¶İ‚·‚é‚È‚çUŒ‚
    owner_.look_target();
    if (to_target_length < owner_.my_info().attack_data.find(LichMotion::Attack1)->second.detection_radius) {
        next_state_timer_ = owner_.info().attack_lottery_by_attack;
        // 60%
        if (MyRandom::random_float(0.0f, 1.0f) <= 0.6f) {
            owner_.change_state((GSuint)LichStateType::Attack, LichMotion::Attack1, false);
        }
        else if (owner_.is_attack_motion(LichMotion::Spell2)) {
            owner_.change_state((GSuint)LichStateType::Spell, LichMotion::Spell2, false);
        }
    }
    // ‰“‹——£‚È‚ç–‚–@
    else {
        // 50%
        if (MyRandom::random_float(0.0f, 1.0f) <= 0.5f && owner_.is_attack_motion(LichMotion::Spell1)) {
            next_state_timer_ = owner_.info().attack_lottery_by_spell;
            owner_.change_state((GSuint)LichStateType::Spell, LichMotion::Spell1, false);
        }
        else if (owner_.is_attack_motion(LichMotion::Spell2)) {
            next_state_timer_ = owner_.info().attack_lottery_by_attack;
            owner_.change_state((GSuint)LichStateType::Spell, LichMotion::Spell2, false);
        }
    }
}

void LichIdleState::exit() {

}
