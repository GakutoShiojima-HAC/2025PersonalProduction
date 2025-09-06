// -----------------------------------------------------------------------------------------
//  File          : Player.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/11
//  Updated       : 2025/08/11
//  Description   : �v���C���[
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Input/Input.h"
#include "Weapon/WeaponManager.h"

class PlayerCamera;

class Player : public Character {
public:
	Player(IWorld* world, const GSvector3& position, const GSvector3& lookat, PlayerCamera* camera);

public:
	enum Motion {
		Idle = 14,			// �ҋ@
		Jump = 17,          // �W�����v
		Fall = 16,          // ��
		Land = 15,			// ���n
		Dead = 9,           // ���S

		HurtF = 5,
		HurtL = 6,
		HurtR = 7,
		HurtB = 4,

		AvoidF = 11,
		AvoidL = 12,
		AvoidR = 13,
		AvoidB = 10,

		WalkF = 31,
		WalkFL = 32,
		WalkFR = 33,
		WalkL = 34,
		WalkR = 35,
		WalkB = 28,
		WalkBL = 29,
		WalkBR = 30,

		SprintF = 21,
		SprintFL = 22,
		SprintFR = 23,
		SprintL = 24,
		SprintR = 25,
		SprintB = 18,
		SprintBL = 19,
		SprintBR = 20,

		Attack1 = 0,
		Attack2 = 1,
		Attack3 = 2,
		Attack4 = 3,

		Skill = 101,          // TODO �X�L��
	};

public:
	void update(float delta_time) override;

	void late_update(float delta_time) override;

	void draw() const override;

	void draw_gui() const override;

public:
	void take_damage(Actor& other, const int damage) override;

	bool is_dead_state() const override;

private:
	void react(Actor& other) override;

	void add_state() override;

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
	/// �Ǐ]�J�����̍X�V
	/// </summary>
	void update_lockon_camera();

	/// <summary>
	/// �U������
	/// </summary>
	void on_attack();

	/// <summary>
	/// �������
	/// </summary>
	void on_avoid();

	/// <summary>
	/// �X�L������
	/// </summary>
	void on_skill();

	/// <summary>
	/// �C���^���N�g����
	/// </summary>
	void on_interact();

	/// <summary>
	/// �U���i��
	/// </summary>
	/// <returns>�Q��</returns>
	int& attack_count();

	/// <summary>
	/// �U�����玟�̍U���ɓ���܂ł̍ŒZ���Ԃ��擾
	/// </summary>
	/// <returns>����</returns>
	float get_enter_next_attack_animation_time();

	/* �A�N�V�����ɂ���ď�Ԃ��ς��A�N�V�����̓��͌��m */
public:
	bool is_attack();
	
	bool is_jump() const;

	bool is_avoid() const;

	bool is_skill() const;

	bool is_interact() const;

	/* �A�N�V�����̃��[�V�����ԍ���ԋp */
public:
	GSuint get_attack_motion();

	GSuint get_skill_motion() const;

	GSuint get_current_motion() const;

private:
	/// <summary>
	/// �U������𐶐�
	/// </summary>
	void generate_attack_collider();

private:
	/// <summary>
	/// �U���A�j���[�V�����C�x���g��ǉ�
	/// </summary>
	void add_attack_animation_event();

private:
	// �v���C���[�J����
	PlayerCamera* camera_{ nullptr };

	// �Ō�̈ړ����x
	float move_speed_{ 0.0f };

private:
	Input& input_ = Input::get_instance();

	// ����Ǘ�
	WeaponManager weapon_manager_;
	// �U���i��
	int attack_count_{ 0 };
	// ���݂̕��� // TODO item�N���X�ɂ�����
	WeaponType weapon_type_{ WeaponType::PlayerSword };
};

using PlayerMotion = Player::Motion;

#endif
