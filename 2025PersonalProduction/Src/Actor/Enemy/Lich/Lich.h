// -----------------------------------------------------------------------------------------
//  File          : Lich.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/21
//  Updated       : 2025/11/21
//  Description   : Lichボス用
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LICH_H_
#define LICH_H_

#include "Actor/Enemy/MyEnemy.h"

struct LichInfo {
    float attack_lottery_by_attack{ 1.0f }; // 近接攻撃をした後に行動抽選に入る時間
    float attack_lottery_by_spell{ 2.0f };  // 遠距離攻撃をした後に行動抽選に入る時間
};

class Lich : public MyEnemy {
public:
    Lich(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info = MyEnemyInfo{}, const LichInfo& info = LichInfo{});

    ~Lich() = default;

public:
    enum Motion {
        Idle = 5,
        hurt = 4,
        Dead = 2,
        Attack1 = 0,
        Attack2 = 1,
        Spell1 = 9,
        Spell2 = 12,
    };

public:
    void draw_gui() const override;

public:
    void take_damage(Actor& other, const int damage) override;

    //void on_hit_attack(AttackCollider& collider) override;

    bool is_dead_state() const override;

    void change_state_and_motion(const GSuint state_num) override;

private:
    void add_state() override;

    //void on_air() override;

    //void on_ground() override;

    //bool is_root_motion_state() const override;

public:
    /// <summary>
    /// 情報を取得
    /// </summary>
    /// <returns>参照</returns>
    const LichInfo& info() const;

private:
    /// <summary>
    /// 魔法Aを召喚
    /// </summary>
    void generate_spell_a();

    /// <summary>
    /// 魔法Bを召喚
    /// </summary>
    void generate_spell_b();

private:
    LichInfo info_;

};

using LichMotion = Lich::Motion;

#endif
