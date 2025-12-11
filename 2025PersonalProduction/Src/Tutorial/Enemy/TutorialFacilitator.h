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
    void draw_gui() const override;

public:
	void take_damage(Actor& other, const int damage) override;

	//void on_hit_attack(AttackCollider& collider) override;

	bool is_dead_state() const override;

    void change_state_and_motion(const GSuint state_num) override;

public:
    /// <summary>
    /// 回避演出中かどうか
    /// </summary>
    /// <returns>演出中なら真を返却</returns>
    bool is_avoid_effect() const;

    /// <summary>
    /// 攻撃チュートリアルイベントの開始
    /// </summary>
    void on_attack_input_event();

    /// <summary>
    /// 回避チュートリアルイベントの開始
    /// </summary>
    void on_avoid_input_event();

    /// <summary>
    /// ゲーム情報GUIを描画するかどうか
    /// </summary>
    /// <returns>参照</returns>
    bool& enable_game_info_gui();

private:
	void add_state() override;

    GSuint event_texture_id_{ 0 };
};

using TutorialFacilitatorMotion = TutorialFacilitator::Motion;

#endif
