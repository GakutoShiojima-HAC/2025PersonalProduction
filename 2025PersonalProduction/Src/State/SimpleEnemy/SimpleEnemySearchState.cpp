#include "State/SimpleEnemy/SimpleEnemySearchState.h"
#include "Actor/Enemy/SimpleEnemy.h"
#include "GameConfig.h"

SimpleEnemySearchState::SimpleEnemySearchState(SimpleEnemy& owner) : SimpleEnemyState(owner) {
    // ���[�V�������擾
    to_find_state_motion_ = owner_.get_motion(SimpleEnemyStateType::Find, &to_find_state_loop_);

    move_timer_ = 0.0f;
}

void SimpleEnemySearchState::update(float delta_time) {
    // �^�[�Q�b�g������������
    if (owner_.search_target()) {
        // �^�[�Q�b�g�̕���������
        owner_.change_state((GSuint)SimpleEnemyStateType::Find, to_find_state_motion_, to_find_state_loop_);
        return;
    }

    // �����Ă��Ȃ��Ԃ͒T���s�����s��
    move_timer_ += delta_time / cFPS;
    if (move_timer_ > 5.0f) {
        // �E������
        rotate_foward_ = owner_.transform().right();
        move_timer_ = 0.0f;
    }

    // �T���s���̉�]
    GSquaternion rotation = GSquaternion::rotateTowards(
        owner_.transform().rotation(),
        GSquaternion::lookRotation(rotate_foward_),
        3.0f
    );
    owner_.transform().rotation(rotation);
}
