// -----------------------------------------------------------------------------------------
//  File          : Player.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/11
//  Updated       : 2025/11/27
//  Description   : プレイヤー
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Input/Input.h"
#include "Item/ItemDataManager.h"

class PlayerCamera;
class Inventory;

/// <summary>
/// 攻撃判定作成アニメーションイベント登録用構造体
/// </summary>
struct PlayerGenerateAttackColliderEvent {
    GSvector3 offset{ 0.0f, 0.0f, 0.0f };   // 生成位置
    float radius{ 1.0f };                   // 半径
    float time{ 0.0f };                     // タイミング
};

/// <summary>
/// エフェクト生成アニメーションイベント登録用構造体
/// </summary>
struct PlayerGenerateEffectEvent {
    bool enabled{ false };
    GSuint effect_id{ 0 };
    GSvector3 offset{ 0.0f, 0.0f, 0.0f };   // 生成位置
    GSvector3 rotate{ 0.0f, 0.0f, 0.0f };   // 回転量
    GSvector3 scale{ 1.0f, 1.0f, 1.0f };    // スケール
    float speed{ 1.0f };                    // 再生速度
    float time{ 0.0f };                     // タイミング
};

/// <summary>
/// 通常攻撃のパラメータ
/// </summary>
struct PlayerAttackParam {
    int damage{ 0 };            // ダメージ
    float next_start{ 0.0f };   // 次の攻撃が発動できる時間
    float next_end{ 0.0f };     // 次の攻撃が発動できなくなる時間
};

/// <summary>
/// 外部データ管理用情報引き渡し構造体
/// </summary>
struct PlayerInfo {
    std::vector<std::vector<PlayerGenerateAttackColliderEvent>> attack_event;
    std::vector<std::vector<PlayerGenerateEffectEvent>> attack_effect_event;
    std::vector<PlayerAttackParam> attack_param;

    std::vector<PlayerGenerateAttackColliderEvent> skill_event;
    std::vector<PlayerGenerateEffectEvent> skill_effect_event;
    int skill_damage{ 0 };

    std::vector<PlayerGenerateAttackColliderEvent> avoid_attack_event;
    std::vector<PlayerGenerateEffectEvent> avoid_attack_effect_event;
    int avoid_attack_damage{ 0 };

    std::vector<PlayerGenerateAttackColliderEvent> avoid_success_attack_event;
    std::vector<PlayerGenerateEffectEvent> avoid_success_attack_effect_event;
    int avoid_success_attack_damage{ 0 };

    std::vector<PlayerGenerateAttackColliderEvent> avoid_success_skill_event;
    std::vector<PlayerGenerateEffectEvent> avoid_success_skill_effect_event;
    int avoid_success_skill_damage{ 0 };
};

class Player : public Character {
public:
	Player(IWorld* world, const GSvector3& position, const GSvector3& rotate, const PlayerInfo& info);

public:
    enum Motion {
        Idle = 0,

        WalkF = 1,
        WalkFL = 12,
        WalkFR = 22,
        WalkL = 23,
        WalkR = 24,
        WalkB = 25,
        WalkBL = 26,
        WalkBR = 27,

        SprintF = 28,
        SprintFL = 2,
        SprintFR = 3,
        SprintL = 4,
        SprintR = 5,
        SprintB = 6,
        SprintBL = 7,
        SprintBR = 8,

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

        Skill = 101,          // TODO スキル
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

    void on_jump() override;

private:
	void react(Actor& other) override;

	void add_state() override;

	void on_air() override;

	void on_ground() override;

    bool is_root_motion_state() const override;

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
    /// 移動速度をリセットする
    /// </summary>
    void reset_move_speed();

    /// <summary>
    /// 移動入力を行っているかどうか
    /// </summary>
    /// <returns>行っていたら真を返却</returns>
    bool is_move_input() const;

	/// <summary>
	/// 追従カメラの更新
	/// </summary>
	void update_lockon_camera();

    /// <summary>
    /// 一番近くの敵を向く
    /// </summary>
    void look_target();

    /// <summary>
    /// タイムラインを停止させる
    /// </summary>
    void stop_timeline();

public:
    /// <summary>
    /// ステートが遷移条件に使えるアクションを行っているかどうか
    /// </summary>
    /// <param name="action">= アクション</param>
    /// <returns>行っていたら真を返却</returns>
    bool is_action(InputAction action) const;

	/// <summary>
	/// 通常攻撃の開始
	/// </summary>
	void attack_start();

	/// <summary>
	/// 回避処理
	/// </summary>
	void on_avoid();

    /// <summary>
    /// 回避攻撃処理
    /// </summary>
    void on_avoid_attack();

	/// <summary>
	/// スキル処理
	/// </summary>
	void on_skill();

	/// <summary>
	/// 現在の攻撃段数
	/// </summary>
	/// <returns>参照</returns>
	int& attack_count();

	/// <summary>
	/// 攻撃から次の攻撃に入るまでの最短時間を取得
	/// </summary>
	/// <returns>delta_time</returns>
	float get_enter_next_attack_min_time() const;

    /// <summary>
    /// 攻撃から次の攻撃に入るまでの最短時間を取得
    /// </summary>
    /// <returns>delta_time</returns>
    float get_enter_next_attack_max_time() const;

    /// <summary>
    /// タイムスケールを受けるかどうか
    /// </summary>
    /// <returns>参照</returns>
    bool& enable_timescale();

    /// <summary>
    /// 武器を描画するかどうかを設定
    /// </summary>
    /// <param name="enabled">= 描画するなら真</param>
    void set_draw_weapon(bool enabled);

    /* アクションのモーション番号を返却 */
public:
	GSuint get_attack_motion() const;

	GSuint get_skill_motion() const;

	GSuint get_current_motion() const;

private:
    /// <summary>
    /// 回避演出スタート
    /// </summary>
    void avoid_effect_start();

    /// <summary>
    /// 回避演出の更新
    /// </summary>
    void update_avoid_effect(float delta_time);

    /// <summary>
    /// インタラクトを更新
    /// </summary>
    void update_interact();

    /// <summary>
    /// インタラクトできるアクターを集める
    /// </summary>
    void get_interact_actor_list();

	/// <summary>
	/// 攻撃アニメーションイベントを追加
	/// </summary>
	/// <param name="info">= 生成情報</param>
	void add_attack_animation_event(const PlayerInfo& info);

    /// <summary>
    /// 攻撃判定を作成
    /// </summary>
    /// <param name="offset">= オフセット</param>
    /// <param name="radius">= 半径</param>
    /// <param name="damage">= ダメージ</param>
    /// <param name="name">= 識別名</param>
    void generate_attack_collider(const GSvector3& offset, float radius, int damage, const std::string& name);

private:
	// プレイヤーカメラ
	PlayerCamera* camera_{ nullptr };

	// 最後の移動速度
	float move_speed_{ 0.0f };

private:
	Input& input_ = Input::get_instance();

    // アイテムデータ
    ItemDataManager& item_data_ = ItemDataManager::get_instance();
    // インベントリ
    Inventory& inventory_;
    // インタラクト対象のアクター
    std::vector<Actor*> interact_actors_;
    // インタラクトしているアクターのインデックス
    GSint interact_target_index_{ 0 };

    // 武器を描画するかどうか
    bool draw_weapon_{ false };

    // 最大HP
    int max_hp_{ 0 };

	// 通常攻撃段数
	int attack_count_{ 0 };
    // 通常攻撃のパラメータ
    std::vector<PlayerAttackParam> attack_param_;

	// 回避演出のタイマー
	float avoid_effect_timer_{ 0.0f };
    // 回避演出ハンドル
    int avoid_effect_handle_{ 0 };
};

using PlayerMotion = Player::Motion;

#endif
