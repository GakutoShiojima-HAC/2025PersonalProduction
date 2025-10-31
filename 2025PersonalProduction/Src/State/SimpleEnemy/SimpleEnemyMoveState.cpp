#include "State/SimpleEnemy/SimpleEnemyMoveState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Utils/MyMath.h"

SimpleEnemyMoveState::SimpleEnemyMoveState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyMoveState::enter() {
    owner_.start_move();
}

void SimpleEnemyMoveState::update(float delta_time) {
    // ƒ^[ƒQƒbƒg‚ª‚¢‚È‚©‚Á‚½‚çƒT[ƒ`‚É–ß‚é
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.get_motion(SimpleEnemyStateType::Search), true);
        return;
    }

    // UŒ‚”»’è‚Ì’†‚É‚¢‚é‚©‚Ç‚¤‚©
    GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    if (to_target.magnitude() < owner_.get_info().attack_detection_radius) {
        // “G‚Ì•ûŒü‚ðŒü‚¢‚Ä‚¢‚é‚©‚Ç‚¤‚©
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            // ”»’è“à‚È‚çUŒ‚
            owner_.save_prev_state();
            owner_.change_state((GSuint)SimpleEnemyStateType::Attack, owner_.get_motion(SimpleEnemyStateType::Attack), false);
            return;
        }
        else {
            owner_.update_look_target(delta_time);
        }
    }
    else {
        owner_.update_move(delta_time);
    }
}

void SimpleEnemyMoveState::exit() {
    owner_.end_move();
}
