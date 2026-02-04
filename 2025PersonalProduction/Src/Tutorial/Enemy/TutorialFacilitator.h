// -----------------------------------------------------------------------------------------
//  File          : TutorialFacilitator.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/09
//  Updated       : 2025/12/09
//  Description   : チュートリアル進行クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TUTORIALFACILITATOR_H_
#define TUTORIALFACILITATOR_H_

#include "Actor/Enemy/MyEnemy.h"

class TutorialFacilitator : public MyEnemy {
public:
	TutorialFacilitator(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info = MyEnemyInfo{});

	~TutorialFacilitator() = default;

public:
    enum Motion {
        Idle = 4,
        Move = 8,
        Attack = 0,
        Hurt = 3,
        Dead = 2
    };

public:
    void update(float delta_time) override;

    void draw_gui() const override;

public:
	void take_damage(Actor& other, const int damage) override;

	//void on_hit_attack(AttackCollider& collider) override;

	bool is_dead_state() const override;

    void change_state_and_motion(const GSuint state_num) override;

private:
    void add_state() override;

public:
    /// <summary>
    /// チュートリアルを初期化
    /// </summary>
    void init_tutorial();

    /// <summary>
    /// 回避空間に入っているかどうか
    /// </summary>
    bool is_avoid_effect() const;

    bool& enable_draw_game_info_gui();

    /// <summary>
    /// 回避エフェクトを一時停止
    /// </summary>
    void pause_avoid_effect();

    /// <summary>
    /// 回避エフェクトを再開
    /// </summary>
    void resume_avoid_effect();

private:
    // 回避方法を教えたかどうか
    bool is_avoid_execute_{ false };
    // ダメージ量で攻撃回数をカウントするかどうか
    bool enable_damage_count_{ true };

    // ターゲットの過去ステート
    unsigned int target_prev_state_{ 0 };

    GSuint event_texture_id_{ 0 };
};

using TutorialFacilitatorMotion = TutorialFacilitator::Motion;

#endif
