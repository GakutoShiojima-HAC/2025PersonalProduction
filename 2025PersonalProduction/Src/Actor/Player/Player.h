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

		Dead = 13,  

        HurtF = 14,
        HurtL = 15,
        HurtR = 16,
        HurtB = 17,

		AvoidF = 18,
		AvoidL = 19,
		AvoidR = 20,
		AvoidB = 21,

		Skill = 101,          // TODO スキル
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

	void on_air() override;

	void on_ground() override;

    void update_mesh(float delta_time) override;

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
	/// 通常攻撃判定を生成
	/// </summary>
	void generate_normal_attack_collider();

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

    /// <summary>
    /// ルートモーションを使う状態かどうか
    /// </summary>
    /// <returns>ルートモーションを使うなら真を返却</returns>
    bool is_root_motion_state() const;

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
