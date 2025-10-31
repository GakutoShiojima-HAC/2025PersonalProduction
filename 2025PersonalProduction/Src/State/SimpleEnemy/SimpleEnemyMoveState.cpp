#include "State/SimpleEnemy/SimpleEnemyMoveState.h"
#include "Actor/Enemy/SimpleEnemy/SimpleEnemy.h"
#include "Engine/Utils/MyMath.h"

SimpleEnemyMoveState::SimpleEnemyMoveState(SimpleEnemy& owner) : SimpleEnemyState(owner) {

}

void SimpleEnemyMoveState::enter() {
    owner_.start_move();
}

void SimpleEnemyMoveState::update(float delta_time) {
    // �^�[�Q�b�g�����Ȃ�������T�[�`�ɖ߂�
    if (!owner_.search_target()) {
        owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.get_motion(SimpleEnemyStateType::Search), true);
        return;
    }

    // �U������̒��ɂ��邩�ǂ���
    const GSvector3 to_target = owner_.target()->transform().position() - owner_.transform().position();
    const float to_target_length = to_target.magnitude();
    if (to_target_length < owner_.get_info().attack_detection_radius) {
        // �G�̕����������Ă��邩�ǂ���
        if (MyMath::to_target_angle(owner_.transform().position(), owner_.transform().forward(), owner_.target()->transform().position()) <= 10.0f) {
            // ������Ȃ�U��
            owner_.save_prev_state();
            owner_.change_state((GSuint)SimpleEnemyStateType::Attack, owner_.get_motion(SimpleEnemyStateType::Attack), false);
            return;
        }
        else {
            owner_.update_look_target(delta_time);
        }
    }
    else {
        // ����������T�[�`��Ԃɖ߂�
        if (to_target_length > owner_.get_info().search_length * 2.5f) {
            owner_.release_target();
            owner_.change_state((GSuint)SimpleEnemyStateType::Search, owner_.get_motion(SimpleEnemyStateType::Search), true);
            return;
        }
        owner_.update_move(delta_time);
    }
}

void SimpleEnemyMoveState::exit() {
    owner_.end_move();
}
