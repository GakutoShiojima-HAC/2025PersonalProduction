// -----------------------------------------------------------------------------------------
//  File          : Player.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/11
//  Updated       : 2025/10/28
//  Description   : �v���C���[
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Input/Input.h"
#include "Item/ItemDataManager.h"

class PlayerCamera;
class Inventory;

/// <summary>
/// �U������쐬�A�j���[�V�����C�x���g�o�^�p�\����
/// </summary>
struct PlayerGenerateAttackColliderEvent {
    GSvector3 offset{ 0.0f, 0.0f, 0.0f };   // �����ʒu
    float radius{ 1.0f };                   // ���a
    float time{ 0.0f };                     // �^�C�~���O
};

/// <summary>
/// �ʏ�U���̃p�����[�^
/// </summary>
struct PlayerAttackParam {
    int damage{ 0 };            // �_���[�W
    float next_start{ 0.0f };   // ���̍U���������ł��鎞��
    float next_end{ 0.0f };     // ���̍U���������ł��Ȃ��Ȃ鎞��
};

/// <summary>
/// �O���f�[�^�Ǘ��p�������n���\����
/// </summary>
struct PlayerInfo {
    std::vector<std::vector<PlayerGenerateAttackColliderEvent>> attack_event;
    std::vector<PlayerAttackParam> attack_param;

    std::vector<PlayerGenerateAttackColliderEvent> skill_event;
    int skill_damage{ 0 };

    std::vector<PlayerGenerateAttackColliderEvent> avoid_attack_event;
    int avoid_attack_damage{ 0 };

    std::vector<PlayerGenerateAttackColliderEvent> avoid_success_attack_event;
    int avoid_success_attack_damage{ 0 };

    std::vector<PlayerGenerateAttackColliderEvent> avoid_success_skill_event;
    int avoid_success_skill_damage{ 0 };
};

class Player : public Character {
public:
	Player(IWorld* world, const GSvector3& position, const GSvector3& lookat, PlayerCamera* camera, const PlayerInfo& info);

public:
    enum Motion {
        Idle = 0,

        WalkF = 1,
        WalkFL = 2,
        WalkFR = 3,
        WalkL = 4,
        WalkR = 5,
        WalkB = 6,
        WalkBL = 7,
        WalkBR = 8,

        SprintF = 28,
        SprintFL = 12,
        SprintFR = 22,
        SprintL = 23,
        SprintR = 24,
        SprintB = 25,
        SprintBL = 26,
        SprintBR = 27,

        Jump = 9,
        Fall = 10,
        Land = 11,
        Interact = 999, // TODO 
        Dead = 13,

        HurtF = 14,
        HurtL = 15,
        HurtR = 16,
        HurtB = 17,

        AvoidF = 18,
        AvoidL = 19,
        AvoidR = 20,
        AvoidB = 21,

        Attack1 = 30,
        Attack2 = 31,
        Attack3 = 32,
        Attack4 = 33,
        Attack5 = 34,

        Skill = 101,          // TODO �X�L��
        AvoidAttack = 36,
        AvoidSuccessAttack = 29,
        AvoidSuccessSkill = 35,
	};

public:
	void update(float delta_time) override;

	void late_update(float delta_time) override;

	void draw() const override;

	void draw_gui() const override;

public:
	void take_damage(Actor& other, const int damage) override;

    void on_hit_attack(AttackCollider& collider) override;

	bool is_dead_state() const override;

private:
	void react(Actor& other) override;

	void add_state() override;

    void update_mesh(float delta_time) override;

	void on_air() override;

	void on_ground() override;

public:
	/// <summary>
	/// �ړ��̍X�V
	/// </summary>
	void update_move(float delta_time);

	/// <summary>
	/// �󒆂ł̈ړ��̍X�V
	/// </summary>
	void update_move_air(float delta_time);

	/// <summary>
	/// �ړ��X�e�[�g�ɑJ��(���ꏈ�����������߃��b�v)
	/// </summary>
	void to_move_state();

    /// <summary>
    /// �ړ����x�����Z�b�g����
    /// </summary>
    void reset_move_speed();

    /// <summary>
    /// �ړ����͂��s���Ă��邩�ǂ���
    /// </summary>
    /// <returns>�s���Ă�����^��ԋp</returns>
    bool is_move_input() const;

	/// <summary>
	/// �Ǐ]�J�����̍X�V
	/// </summary>
	void update_lockon_camera();

    /// <summary>
    /// ��ԋ߂��̓G������
    /// </summary>
    void look_target();

    /// <summary>
    /// �^�C�����C�����~������
    /// </summary>
    void stop_timeline();

public:
    /// <summary>
    /// �X�e�[�g���J�ڏ����Ɏg����A�N�V�������s���Ă��邩�ǂ���
    /// </summary>
    /// <param name="action">= �A�N�V����</param>
    /// <returns>�s���Ă�����^��ԋp</returns>
    bool is_action(InputAction action) const;

	/// <summary>
	/// �ʏ�U���̊J�n
	/// </summary>
	void attack_start();

	/// <summary>
	/// �������
	/// </summary>
	void on_avoid();

    /// <summary>
    /// ����U������
    /// </summary>
    void on_avoid_attack();

	/// <summary>
	/// �X�L������
	/// </summary>
	void on_skill();

	/// <summary>
	/// ���݂̍U���i��
	/// </summary>
	/// <returns>�Q��</returns>
	int& attack_count();

	/// <summary>
	/// �U�����玟�̍U���ɓ���܂ł̍ŒZ���Ԃ��擾
	/// </summary>
	/// <returns>delta_time</returns>
	float get_enter_next_attack_min_time() const;

    /// <summary>
    /// �U�����玟�̍U���ɓ���܂ł̍ŒZ���Ԃ��擾
    /// </summary>
    /// <returns>delta_time</returns>
    float get_enter_next_attack_max_time() const;

    /// <summary>
    /// �^�C���X�P�[�����󂯂邩�ǂ���
    /// </summary>
    /// <returns>�Q��</returns>
    bool& enable_timescale();

    /* �A�N�V�����̃��[�V�����ԍ���ԋp */
public:
	GSuint get_attack_motion() const;

	GSuint get_skill_motion() const;

	GSuint get_current_motion() const;

private:
    /// <summary>
    /// �U��������쐬
    /// </summary>
    /// <param name="offset">= �I�t�Z�b�g</param>
    /// <param name="radius">= ���a</param>
    /// <param name="damage">= �_���[�W</param>
    /// <param name="name">= ���ʖ�</param>
    void generate_attack_collider(const GSvector3& offset, float radius, int damage, const std::string& name);

    /// <summary>
    /// �C���^���N�g���X�V
    /// </summary>
    void interact_update();

    /// <summary>
    /// �C���^���N�g�ł���A�N�^�[���W�߂�
    /// </summary>
    void get_interact_actor_list();

	/// <summary>
	/// �U���A�j���[�V�����C�x���g��ǉ�
	/// </summary>
	/// <param name="info">= �������</param>
	void add_attack_animation_event(const PlayerInfo& info);

    /// <summary>
    /// ���[�g���[�V�������g����Ԃ��ǂ���
    /// </summary>
    /// <returns>���[�g���[�V�������g���Ȃ�^��ԋp</returns>
    bool is_root_motion_state() const;

private:
	// �v���C���[�J����
	PlayerCamera* camera_{ nullptr };

	// �Ō�̈ړ����x
	float move_speed_{ 0.0f };

private:
	Input& input_ = Input::get_instance();

    // �A�C�e���f�[�^
    ItemDataManager& item_data_ = ItemDataManager::get_instance();
    // �C���x���g��
    Inventory& inventory_;
    // �C���^���N�g�Ώۂ̃A�N�^�[
    std::vector<Actor*> interact_actors_;
    // �C���^���N�g���Ă���A�N�^�[�̃C���f�b�N�X
    GSint interact_target_index_{ 0 };

	// �ʏ�U���i��
	int attack_count_{ 0 };
    // �ʏ�U���̃p�����[�^
    std::vector<PlayerAttackParam> attack_param_;

	// ������o�̃^�C�}�[
	float avoid_effect_timer_{ 0.0f };
};

using PlayerMotion = Player::Motion;

#endif
