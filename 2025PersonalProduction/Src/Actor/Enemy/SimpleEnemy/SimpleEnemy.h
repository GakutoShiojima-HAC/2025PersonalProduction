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

#include "Actor/Enemy/MyEnemy.h"
#include "State/SimpleEnemy/SimpleEnemyState.h"

struct SimpleEnemyInfo {
    GSuint motion_idle{ 0 };        // アイドルのモーション
    GSuint motion_attack{ 1 };      // 攻撃のモーション
    GSuint motion_move{ 2 };        // 移動のモーション
    GSuint motion_hurt{ 3 };        // 負傷のモーション
    GSuint motion_dead{ 4 };        // 死亡のモーション
};

class SimpleEnemy : public MyEnemy {
public:
    SimpleEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& my_info = MyEnemyInfo{}, const SimpleEnemyInfo& info = SimpleEnemyInfo{});

    ~SimpleEnemy() = default;

public:
    void take_damage(Actor& other, const int damage) override;

    void on_hit_attack(AttackCollider& collider) override {};

    bool is_dead_state() const override;

    void change_state_and_motion(const GSuint state_num) override;

private:
    void add_state() override;

    void on_air() override {};

    void on_ground() override {};

public:
    /// <summary>
    /// 情報を取得
    /// </summary>
    /// <returns>参照</returns>
    const SimpleEnemyInfo& info() const;

private:
    SimpleEnemyInfo info_;

};

#endif
