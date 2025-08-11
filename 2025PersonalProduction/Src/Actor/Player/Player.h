#ifndef PLAYER_H_
#define PLAYER_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Input/Input.h"

class PlayerCamera;

class Player : public Character {
public:
	Player(IWorld* world, const GSvector3& position, const GSvector3& lookat, PlayerCamera* camera);

public:
	enum Motion {
		Idle = 0,       // �ҋ@
		Fall,           // ��
		Land,           // ���n

		Move,           // �ړ�
		Interact,       // �C���^���N�g
		Attack,         // �U��
		Skill,          // �X�L��
		Jump,           // �W�����v
		Avoid,          // ���

		Hurt,           // ����
		Dead,           // ���S
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

	/* �A�N�V�����ɂ���ď�Ԃ��ς��A�N�V�����̓��͌��m */
public:
	bool is_attack() const;
	
	bool is_jump() const;

	bool is_avoid() const;

	bool is_skill() const;

	bool is_interact() const;

	/* �A�N�V�����̃��[�V���� */
public:
	GSuint get_attack_motion() const;

	GSuint get_avoid_motion() const;

	GSuint get_skill_motion() const;

private:
	// �v���C���[�J����
	PlayerCamera* camera_{ nullptr };

	// �Ō�̈ړ����x
	float move_speed_{ 0.0f };

private:
	Input& input_ = Input::get_instance();

};

using PlayerMotion = Player::Motion;

#endif
