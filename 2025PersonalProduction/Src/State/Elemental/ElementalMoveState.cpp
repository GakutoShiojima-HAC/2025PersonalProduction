#include "ElementalMoveState.h"
#include "Actor/Enemy/Elemental/Elemental.h"

ElementalMoveState::ElementalMoveState(Elemental& owner) :
    ElementalState(owner) {

}

void ElementalMoveState::enter() {
    owner_.start_move();
}

void ElementalMoveState::update(float delta_time) {
    // ターゲットがいなかったらサーチに戻る
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)ElementalStateType::Idle);
        return;
    }

    // 一定時間移動したら魔法を使う
    if (owner_.state_timer() / cFPS > 5.0f) {
        owner_.look_target();
        owner_.change_state_and_motion((GSuint)ElementalStateType::Spell);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();

    // 遠くに行きすぎたら待機へ
    if (to_target_length > owner_.my_info().search_length * 2.0f) {
        owner_.change_state_and_motion((GSuint)ElementalStateType::Idle);
        return;
    }
    // 攻撃距離に入ったら攻撃へ
    if (to_target_length < owner_.my_info().attack_data.find(owner_.info().motion_attack1)->second.detection_radius) {
        owner_.look_target();
        owner_.change_state_and_motion((GSuint)ElementalStateType::Attack1);
        return;
    }

    owner_.update_move(delta_time);
}

void ElementalMoveState::exit() {
    owner_.move_end();
}
