#include "ElementalAttack1State.h"
#include "Actor/Enemy/Elemental/Elemental.h"

ElementalAttack1State::ElementalAttack1State(Elemental& owner) :
    ElementalState(owner) {

}

void ElementalAttack1State::enter() {
    owner_.look_target();
}

void ElementalAttack1State::update(float delta_time) {
    if (!owner_.is_motion_end()) return;

    // 次の攻撃に移るとき、対象がいなければアイドルへ
    if (!owner_.search_target()) {
        owner_.change_state_and_motion((GSuint)ElementalStateType::Idle);
        return;
    }

    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();
    // 次の攻撃に移るとき、対象が攻撃の届かない距離にいたら攻撃へ
    if (to_target_length < owner_.my_info().attack_data.find(owner_.info().motion_attack2)->second.detection_radius) {
        owner_.change_state_and_motion((GSuint)ElementalStateType::Attack2);
        return;
    }
    // 次の攻撃に移るとき、対象が攻撃の届かない距離にいたらアイドルへ
    else {
        owner_.change_state_and_motion((GSuint)ElementalStateType::Idle);
        return;
    }
}
