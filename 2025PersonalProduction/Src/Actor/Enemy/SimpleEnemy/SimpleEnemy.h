// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/27
//  Updated       : 2025/10/27
//  Description   : �V���v���ȋ���������AI�̃x�[�X�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_H_
#define SIMPLE_ENEMY_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "State/SimpleEnemy/SimpleEnemyState.h"
#include <string>
#include "Engine/Core/NavMesh/NavMeshAgent.h"

struct SimpleEnemyInfo {
    float height{ 2.0f };           // ����
    float foot_offset{ 0.05f };     // ���̃I�t�Z�b�g
    float radius{ 0.5f };           // �Փ˔���̔��a
    
    std::string name{ "SimpleEnemy" };  // ���O
    int hp{ 5 };                    // �̗�
    float move_speed{ 0.25f };      // �ړ����x
    
    GSuint skinmesh{ 0 };           // �X�L�����b�V��ID

    GSuint motion_idle{ 0 };        // �A�C�h���̃��[�V����
    GSuint motion_attack{ 1 };      // �U���̃��[�V����
    GSuint motion_move{ 2 };        // �ړ��̃��[�V����
    GSuint motion_hurt{ 3 };        // �����̃��[�V����
    GSuint motion_dead{ 4 };        // ���S�̃��[�V����

    float search_fov{ 75.0f };      // ���G�̎���p
    float search_length{ 5.0f };    // ���G�̋���

    GSvector3 attack_offset{ 0.0f, 0.5f, 1.0f };    // �U������̐����ʒu
    float attack_event_time{ 20.0f };               // �U������̐����^�C�~���O
    float attack_radius{ 0.5f };    // �U������̔��a
    int attack_damage{ 1 };         // �U����
    float attack_detection_radius{ 1.0f };          // �U���𔭓��ł���ΏۂƂ̋���
};

class SimpleEnemy : public Character {
public:
    SimpleEnemy(IWorld* world, const GSvector3& position, const GSvector3& lookat, const SimpleEnemyInfo& info = SimpleEnemyInfo{});

    ~SimpleEnemy() = default;

public:
    void update(float delta_time) override;

    void draw() const override;

    void draw_gui() const override;

public:
    void take_damage(Actor& other, const int damage) override;

    void on_hit_attack(AttackCollider& collider) override {};

    bool is_dead_state() const override;

private:
    void react(Actor& other) override;

    void add_state() override;

    void update_mesh(float delta_time) override;

    void on_air() override {};

    void on_ground() override {};

public:
    /// <summary>
    /// �����擾
    /// </summary>
    /// <returns>�Q��</returns>
    const SimpleEnemyInfo& get_info() const;

    /// <summary>
    /// ���݂̃X�e�[�g��O�̃X�e�[�g�Ƃ��ĕۑ�
    /// </summary>
    void save_prev_state();

    /// <summary>
    /// �O�̃X�e�[�g�ɖ߂�
    /// </summary>
    void change_prev_state();

    /// <summary>
    /// �O�̃X�e�[�g�ԍ�
    /// </summary>
    /// <returns>�ԍ�</returns>
    GSuint prev_state_num() const;

    /// <summary>
    /// �X�e�[�g�ɑΉ��������[�V�����ԍ���ԋp
    /// </summary>
    /// <param name="state">= ���</param>
    /// <param name="loop">= ���[�v���邩�ǂ���</param>
    /// <returns>���[�V�����ԍ�</returns>
    GSuint get_motion(SimpleEnemyStateType state, bool* loop = nullptr) const;

    /// <summary>
    /// �^�[�Q�b�g��T��
    /// </summary>
    /// <returns>����������^��ԋp</returns>
    bool search_target();

    /// <summary>
    /// �ΏۂƂ���^�[�Q�b�g
    /// </summary>
    /// <returns>�L�����N�^�[</returns>
    Character* target();

    /// <summary>
    /// �ړ��J�n
    /// </summary>
    void start_move();

    /// <summary>
    /// �ړ��̍X�V
    /// </summary>
    void update_move(float delta_time);

    /// <summary>
    /// �ړ��I��
    /// </summary>
    void end_move();

    /// <summary>
    /// �^�[�Q�b�g������
    /// </summary>
    void update_look_target(float delta_time);

    /// <summary>
    /// �^�[�Q�b�g������
    /// </summary>
    void release_target();

private:
    /// <summary>
    /// ���[�g���[�V�������g����Ԃ��ǂ���
    /// </summary>
    /// <returns>���[�g���[�V�������g���Ȃ�^��ԋp</returns>
    bool is_root_motion_state() const;

    /// <summary>
    /// �ʏ�U������𐶐�
    /// </summary>
    void generate_attack_collider();

private:
    SimpleEnemyInfo info_;

    GSuint prev_state_num_{ 0 };

    Character* target_{ nullptr };

    NavMeshAgent navmesh_{};

};

#endif
