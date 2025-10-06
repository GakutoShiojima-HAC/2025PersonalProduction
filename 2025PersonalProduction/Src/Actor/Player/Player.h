// -----------------------------------------------------------------------------------------
//  File          : Player.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/11
//  Updated       : 2025/08/11
//  Description   : プレイヤー
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Input/Input.h"
#include "Item/ItemDataManager.h"
#include "Weapon/WeaponManager.h"

class PlayerCamera;

class Player : public Character {
public:
	Player(IWorld* world, const GSvector3& position, const GSvector3& lookat, PlayerCamera* camera);

public:
	enum Motion {
		Idle = 14,			// 待機
		Jump = 17,          // ジャンプ
		Fall = 16,          // 空中
		Land = 15,			// 着地
		Dead = 9,           // 死亡

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

		Skill = 101,          // TODO スキル
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

	/// <summary>
	/// 攻撃段数
	/// </summary>
	/// <returns>参照</returns>
	int& attack_count();

	/// <summary>
	/// 攻撃から次の攻撃に入るまでの最短時間を取得
	/// </summary>
	/// <returns>時間</returns>
	float get_enter_next_attack_animation_time();

	/* アクションによって状態が変わるアクションの入力検知 */
public:
	bool is_attack();
	
	bool is_jump() const;

	bool is_avoid() const;

	bool is_skill() const;

	bool is_interact() const;

	/* アクションのモーション番号を返却 */
public:
	GSuint get_attack_motion();

	GSuint get_skill_motion() const;

	GSuint get_current_motion() const;

private:
	/// <summary>
	/// 攻撃判定を生成
	/// </summary>
	void generate_attack_collider();

    /// <summary>
    /// インタラクトを更新
    /// </summary>
    void interact_update();

    /// <summary>
    /// インタラクトできるアクターを集める
    /// </summary>
    void get_interact_actor_list();

    /// <summary>
	/// 攻撃アニメーションイベントを追加
	/// </summary>
	void add_attack_animation_event();

private:
	// プレイヤーカメラ
	PlayerCamera* camera_{ nullptr };

	// 最後の移動速度
	float move_speed_{ 0.0f };

private:
	Input& input_ = Input::get_instance();

    // アイテムデータ
    ItemDataManager& item_data_ = ItemDataManager::get_instance();
    // インタラクト対象のアクター
    std::vector<Actor*> interact_actors_;
    // インタラクトしているアクターのインデックス
    GSint interact_target_index_{ 0 };

	// 武器管理
	WeaponManager weapon_manager_;
	// 攻撃段数
	int attack_count_{ 0 };

	// 回避演出のタイマー
	float avoid_effect_timer_{ 0.0f };

    // 右手ボーン
    int right_hand_bone_{ 114 };
};

using PlayerMotion = Player::Motion;

#endif
