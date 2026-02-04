// -----------------------------------------------------------------------------------------
//  File          : Viking.h
//  Author        : Shiojima Gakuto
//  Description   : ボス用
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
//  生成：GSCodeGenerator
// -----------------------------------------------------------------------------------------

#ifndef VIKING_H_
#define VIKING_H_

#include "Actor/Enemy/MyEnemy.h"

struct VikingInfo {
    float leap_power{ 3.0f };   // とびかかり攻撃のジャンプ力
    int landing_damage{ 5 };    // 着地攻撃のダメージ
};

class Viking : public MyEnemy {
public:
    Viking(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info = MyEnemyInfo{});

    ~Viking() = default;

public:
    enum Motion {
        Idle = 16,
        Angry = 0,
        MoveF = 22, //20
        MoveB = 21,
        Hurt = 15,
        Dead = 13,
        Block = 11,
        BlockHit = 12,
        Attack = 1,
        JumpStart = 19,
        Jumping = 18,
        JumpEnd = 17,
        RushCharge = 3,
        Rushing = 5,
        RushStart = 2,
        RushEnd = 4
    };

public:
    void draw_gui() const override;

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
    const VikingInfo& info() const;

    void dash_effect_start();

    void dash_effect_end();

    void update_dash_effect(float delta_time);

private:
    void generate_land_attack();

private:
    VikingInfo info_;

    int dash_effect_handle_{ 0 };
    bool dash_effect_enabled_{ false };

};

using VikingMotion = Viking::Motion;

#endif
