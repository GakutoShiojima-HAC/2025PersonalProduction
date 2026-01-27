// -----------------------------------------------------------------------------------------
//  File          : LeapEnemy.h
//  Author        : Shiojima Gakuto
//  Description   : とびかかり攻撃を持つ雑魚
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
//  生成：GSCodeGenerator
// -----------------------------------------------------------------------------------------

#ifndef LEAP_ENEMY_H_
#define LEAP_ENEMY_H_

#include "Actor/Enemy/MyEnemy.h"

struct LeapEnemyInfo {
    GSuint motion_idle{ 0 };        // アイドルのモーション
    GSuint motion_move{ 2 };        // 移動のモーション
    GSuint motion_hurt{ 3 };        // 負傷のモーション
    GSuint motion_dead{ 4 };        // 死亡のモーション
    GSuint motion_attack{ 5 };
    GSuint motion_leap_start{ 6 };
    GSuint motion_leaping{ 7 };
    GSuint motion_leapend{ 8 };

    float leap_power{ 1.5f };   // とびかかり攻撃のジャンプ力
    int landing_damage{ 1 };    // 着地攻撃のダメージ
};

class LeapEnemy : public MyEnemy {
public:
    LeapEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info = MyEnemyInfo{}, const LeapEnemyInfo& info = LeapEnemyInfo{});

    ~LeapEnemy() = default;

public:
    void take_damage(Actor& other, const int damage) override;

    bool is_dead_state() const override;

    void change_state_and_motion(const GSuint state_num) override;

private:
    void add_state() override;

    //void on_air() override;

    //void on_ground() override;

    bool is_root_motion_state() const override;

public:
    /// <summary>
    /// 情報を取得
    /// </summary>
    /// <returns>参照</returns>
    const LeapEnemyInfo& info() const;

private:
    void generate_land_attack();

private:
    LeapEnemyInfo info_;

};

#endif
