// -----------------------------------------------------------------------------------------
//  File          : MyEnemy.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/21
//  Updated       : 2025/11/21
//  Description   : 敵のベースクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MY_ENEMY_H_
#define MY_ENEMY_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/NavMesh/NavMeshAgent.h"
#include <string>

struct MyEnemyInfo {
    std::string name{ "MyEnemy" };      // 名前
    std::string type{ "Normal" };       // タイプ

    GSuint skinmesh{ 0 };               // スキンメッシュID

    int hp{ 5 };                        // 体力
    float move_speed{ 0.125f };         // 移動速度

    float search_fov{ 75.0f };          // 索敵の視野角
    float search_length{ 5.0f };        // 索敵の距離

    GSvector3 attack_offset{ 0.0f, 0.5f, 1.0f };    // 攻撃判定の生成位置
    float attack_event_time{ 20.0f };               // 攻撃判定の生成タイミング
    float attack_radius{ 0.5f };                    // 攻撃判定の半径
    int attack_damage{ 1 };                         // 攻撃力
    float attack_detection_radius{ 1.0f };          // 攻撃を発動できる対象との距離

    float falter_rate{ 1.0f };  // 攻撃を受けた時に怯む確率(0.0~1.0)
};

class MyEnemy : public Character {
public:
    MyEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& info);

    ~MyEnemy() = default;

public:
    virtual void update(float delta_time) override;

    virtual void draw() const override;

protected:
    virtual void react(Actor& other) override;

    virtual void update_mesh(float delta_time) override;

public:
    /// <summary>
    /// 指定したステートと対応したモーションに変更
    /// </summary>
    /// <param name="state_num">ステート番号</param>
    virtual void change_state_and_motion(const GSuint state_num) = 0;

protected:
    /// <summary>
    /// ルートモーションを使う状態かどうか
    /// </summary>
    /// <returns>ルートモーションを使うなら真を返却</returns>
    virtual bool is_root_motion_state() const = 0;

public:
    /// <summary>
    /// 現在のステートを保存
    /// </summary>
    void save_current_state();

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
    /// ターゲットを探す
    /// </summary>
    /// <returns>見つかったら真を返却</returns>
    virtual bool search_target();

    /// <summary>
    /// 対象とするターゲット
    /// </summary>
    /// <returns>キャラクター</returns>
    Character* target();

    /// <summary>
    /// 移動開始
    /// </summary>
    bool start_move();

    /// <summary>
    /// 指定地点への移動開始
    /// </summary>
    /// <param name="to">= 指定地点</param>
    /// <returns>移動できるなら真を返却</returns>
    bool start_move(const GSvector3& to);

    /// <summary>
    /// 移動の更新
    /// </summary>
    void update_move(float delta_time);

    /// <summary>
    /// 移動が終了したかどうか
    /// </summary>
    /// <returns>終了してたら真を返却</returns>
    bool is_move_end() const;

    /// <summary>
    /// 移動終了
    /// </summary>
    void move_end();

    /// <summary>
    /// ターゲットを見る
    /// </summary>
    void update_look_target(float delta_time, float angle = 3.0f);

    /// <summary>
    /// ターゲットを解除
    /// </summary>
    void release_target();

    /// <summary>
    /// 初期座標
    /// </summary>
    /// <returns>座標</returns>
    GSvector3& origin_position();

    /// <summary>
    /// 情報を取得
    /// </summary>
    /// <returns></returns>
    const MyEnemyInfo& my_info() const;

protected:
    /// <summary>
    /// 通常攻撃判定を生成
    /// </summary>
    void generate_attack_collider();

protected:
    GSuint prev_state_num_{ 0 };
    GSuint prev_motion_num_{ 0 };
    bool prev_motion_loop_{ false };

    Character* target_{ nullptr };

    NavMeshAgent navmesh_{};

    GSvector3 origin_position_{ 0.0f, 0.0f, 0.0f };

    MyEnemyInfo my_info_;
};        

#endif
