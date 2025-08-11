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
		Idle = 0,       // 待機
		Fall,           // 空中
		Land,           // 着地

		Move,           // 移動
		Interact,       // インタラクト
		Attack,         // 攻撃
		Skill,          // スキル
		Jump,           // ジャンプ
		Avoid,          // 回避

		Hurt,           // 負傷
		Dead,           // 死亡
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
	/// 移動の更新
	/// </summary>
	void update_move(float delta_time);

	/// <summary>
	/// 空中での移動の更新
	/// </summary>
	void update_move_air(float delta_time);

	/// <summary>
	/// 移動ステートに遷移(同一処理が多いためラップ)
	/// </summary>
	void to_move_state();

	/// <summary>
	/// 追従カメラの更新
	/// </summary>
	void update_lockon_camera();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void on_attack();

	/// <summary>
	/// 回避処理
	/// </summary>
	void on_avoid();

	/// <summary>
	/// スキル処理
	/// </summary>
	void on_skill();

	/// <summary>
	/// インタラクト処理
	/// </summary>
	void on_interact();

	/* アクションによって状態が変わるアクションの入力検知 */
public:
	bool is_attack() const;
	
	bool is_jump() const;

	bool is_avoid() const;

	bool is_skill() const;

	bool is_interact() const;

	/* アクションのモーション */
public:
	GSuint get_attack_motion() const;

	GSuint get_avoid_motion() const;

	GSuint get_skill_motion() const;

private:
	// プレイヤーカメラ
	PlayerCamera* camera_{ nullptr };

	// 最後の移動速度
	float move_speed_{ 0.0f };

private:
	Input& input_ = Input::get_instance();

};

using PlayerMotion = Player::Motion;

#endif
