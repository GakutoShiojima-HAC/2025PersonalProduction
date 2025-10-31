// -----------------------------------------------------------------------------------------
//  File          : SimpleEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/27
//  Updated       : 2025/10/27
//  Description   : シンプルな挙動をするAIのベースクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SIMPLE_ENEMY_H_
#define SIMPLE_ENEMY_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "State/SimpleEnemy/SimpleEnemyState.h"
#include <string>
#include "Engine/Core/NavMesh/NavMeshAgent.h"

struct SimpleEnemyInfo {
    float height{ 2.0f };           // 高さ
    float foot_offset{ 0.05f };     // 足のオフセット
    float radius{ 0.5f };           // 衝突判定の半径
    
    std::string name{ "SimpleEnemy" };  // 名前
    int hp{ 5 };                    // 体力
    float move_speed{ 0.25f };      // 移動速度
    
    GSuint skinmesh{ 0 };           // スキンメッシュID

    GSuint motion_idle{ 0 };        // アイドルのモーション
    GSuint motion_attack{ 1 };      // 攻撃のモーション
    GSuint motion_move{ 2 };        // 移動のモーション
    GSuint motion_hurt{ 3 };        // 負傷のモーション
    GSuint motion_dead{ 4 };        // 死亡のモーション

    float search_fov{ 75.0f };      // 索敵の視野角
    float search_length{ 5.0f };    // 索敵の距離

    GSvector3 attack_offset{ 0.0f, 0.5f, 1.0f };    // 攻撃判定の生成位置
    float attack_event_time{ 20.0f };               // 攻撃判定の生成タイミング
    float attack_radius{ 0.5f };    // 攻撃判定の半径
    int attack_damage{ 1 };         // 攻撃力
    float attack_detection_radius{ 1.0f };          // 攻撃を発動できる対象との距離
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
    /// 情報を取得
    /// </summary>
    /// <returns>参照</returns>
    const SimpleEnemyInfo& get_info() const;

    /// <summary>
    /// 現在のステートを前のステートとして保存
    /// </summary>
    void save_prev_state();

    /// <summary>
    /// 前のステートに戻る
    /// </summary>
    void change_prev_state();

    /// <summary>
    /// 前のステート番号
    /// </summary>
    /// <returns>番号</returns>
    GSuint prev_state_num() const;

    /// <summary>
    /// ステートに対応したモーション番号を返却
    /// </summary>
    /// <param name="state">= 状態</param>
    /// <param name="loop">= ループするかどうか</param>
    /// <returns>モーション番号</returns>
    GSuint get_motion(SimpleEnemyStateType state, bool* loop = nullptr) const;

    /// <summary>
    /// ターゲットを探す
    /// </summary>
    /// <returns>見つかったら真を返却</returns>
    bool search_target();

    /// <summary>
    /// 対象とするターゲット
    /// </summary>
    /// <returns>キャラクター</returns>
    Character* target();

    /// <summary>
    /// 移動開始
    /// </summary>
    void start_move();

    /// <summary>
    /// 移動の更新
    /// </summary>
    void update_move(float delta_time);

    /// <summary>
    /// 移動終了
    /// </summary>
    void end_move();

    /// <summary>
    /// ターゲットを見る
    /// </summary>
    void update_look_target(float delta_time);

    /// <summary>
    /// ターゲットを解除
    /// </summary>
    void release_target();

private:
    /// <summary>
    /// ルートモーションを使う状態かどうか
    /// </summary>
    /// <returns>ルートモーションを使うなら真を返却</returns>
    bool is_root_motion_state() const;

    /// <summary>
    /// 通常攻撃判定を生成
    /// </summary>
    void generate_attack_collider();

private:
    SimpleEnemyInfo info_;

    GSuint prev_state_num_{ 0 };

    Character* target_{ nullptr };

    NavMeshAgent navmesh_{};

};

#endif
