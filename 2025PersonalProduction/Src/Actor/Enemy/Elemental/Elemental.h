// -----------------------------------------------------------------------------------------
//  File          : Elemental.h
//  Author        : Shiojima Gakuto
//  Description   : ボス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
//  生成：GSCodeGenerator
// -----------------------------------------------------------------------------------------

#ifndef ELEMENTAL_H_
#define ELEMENTAL_H_

#include "Actor/Enemy/MyEnemy.h"

enum class ElementalSpellType {
    None,
    FireBall,       // 火球
    ExplodeRoad,    // 地面連続爆発攻撃
};

struct ElementalInfo {
    GSuint motion_idle{ 0 };        // アイドルのモーション
    GSuint motion_angry{ 1 };       // 発見時のモーション
    GSuint motion_move{ 2 };        // 移動のモーション
    GSuint motion_hurt{ 3 };        // 負傷のモーション
    GSuint motion_dead{ 4 };        // 死亡のモーション
    GSuint motion_attack1{ 5 };
    GSuint motion_attack2{ 6 };
    GSuint motion_spell{ 7 };

    // 魔法の種類
    ElementalSpellType spell_type{ ElementalSpellType::None };
};

class Elemental : public MyEnemy {
public:
    Elemental(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info = MyEnemyInfo{}, const ElementalInfo& info = ElementalInfo{});

    ~Elemental() = default;

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
    const ElementalInfo& info() const;

private:
    void generate_spell_attack();

private:
    ElementalInfo info_;

};

#endif
