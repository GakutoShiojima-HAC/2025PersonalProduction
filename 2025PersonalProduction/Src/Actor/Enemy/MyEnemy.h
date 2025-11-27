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
#include <unordered_map>

struct MyEnemyAttackData {
    float start_time{ 0.0f };               // 攻撃開始のタイミング(演出用)
    float attack_time{ 0.0f };              // 攻撃判定生成のタイミング
    GSvector3 offset{ 0.0f, 0.0f, 0.0f };   // 攻撃判定生成位置
    float radius{ 0.0f };                   // 攻撃判定の半径
    int damage{ 0 };                        // 攻撃ダメージ
    float detection_radius{ 0.0f };         // 攻撃判定を生成できる距離
    GSuint bone{ 0 };                       // 攻撃を行うボーン(演出用)
};

struct MyEnemyInfo {
    std::string name{ "MyEnemy" };      // 名前
    std::string pawn_type{ "Normal" };  // ポーンタイプ
    GSuint skinmesh{ 0 };               // スキンメッシュID
    float move_speed{ 0.125f };         // 移動速度
    float search_fov{ 75.0f };          // 索敵の視野角
    float search_length{ 5.0f };        // 索敵の距離

    int hp{ 5 };                        // 体力
    int damage{ 1 };                    // 基礎攻撃ダメージ
    int critical_bone_num{ 0 };         // 急所(演出用)ボーン番号
    std::unordered_map<GSuint, MyEnemyAttackData> attack_data; // 攻撃データ(motion, data)

    float falter_rate{ 1.0f };          // 攻撃を受けた時に怯む確率(0.0~1.0)
    float ui_height{ 1.0f };            // 頭の上に出すUI表示の高さ
};

class MyEnemy : public Character {
public:
    MyEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& info);

    ~MyEnemy() = default;

public:
    virtual void update(float delta_time) override;

    virtual void late_update(float delta_time) override;

    virtual void draw() const override;

    virtual void draw_gui() const override;

    virtual bool is_attack_soon() const override;

protected:
    virtual void react(Actor& other) override;

public:
    /// <summary>
    /// 指定したステートと対応したモーションに変更
    /// </summary>
    /// <param name="state_num">ステート番号</param>
    virtual void change_state_and_motion(const GSuint state_num) = 0;

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
    /// ターゲットを見る(即時)
    /// </summary>
    void look_target();

    /// <summary>
    /// ターゲットを解除
    /// </summary>
    void release_target();

    /// <summary>
    /// 初期座標を取得
    /// </summary>
    /// <returns>参照</returns>
    GSvector3& origin_position();

    /// <summary>
    /// 情報を取得
    /// </summary>
    /// <returns>const参照</returns>
    const MyEnemyInfo& my_info() const;

    /// <summary>
    /// 指定したモーションが攻撃モーションかどうか
    /// </summary>
    /// <param name="motion">= モーション番号</param>
    /// <returns>攻撃モーションなら真を返却</returns>
    bool is_attack_motion(GSuint motion) const;

protected:
    /// <summary>
    /// モーションによる攻撃アニメーションイベントを生成
    /// </summary>
    /// <param name="motion">= モーション</param>
    /// <param name="data">= 攻撃データ</param>
    void set_motion_attack_event(GSuint motion, const MyEnemyAttackData& data);

    /// <summary>
    /// 攻撃判定を生成
    /// </summary>
    /// <param name="offset">= オフセット</param>
    /// <param name="radius">= 半径</param>
    /// <param name="damage">= ダメージ</param>
    void generate_attack_collider(const GSvector3& offset, float radius, int damage);

    /// <summary>
    /// HPを描画
    /// </summary>
    void draw_hp_gauge() const;

    /// <summary>
    /// ボスバーを描画
    /// </summary>
    void draw_boss_bar() const;

    /// <summary>
    /// モーションによるアラートアニメーションイベントを作成
    /// </summary>
    /// <param name="motion"= モーション</param>
    /// <param name="time">= 再生タイミング</param>
    /// <param name="bone_num">= ボーン番号</param>
    void set_motion_alert_event(GSuint motion, float time, GSuint bone_num);

    /// <summary>
    /// アラートを再生
    /// </summary>
    /// <param name="bone_num">= ボーン番号</param>
    void play_alert_event(GSuint bone_num);

    /// <summary>
    /// アラートエフェクトの更新
    /// </summary>
    void update_alert_effect();

protected:
    GSuint prev_state_num_{ 0 };
    GSuint prev_motion_num_{ 0 };
    bool prev_motion_loop_{ false };

    Character* target_{ nullptr };

    NavMeshAgent navmesh_;

    GSvector3 origin_position_{ 0.0f, 0.0f, 0.0f };

    MyEnemyInfo my_info_;

    int alert_effect_handle_{ 0 };
    int alert_effect_bone_num_{ 0 };
};        

#endif
