// -----------------------------------------------------------------------------------------
//  File          : Heal.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/03
//  Updated       : 2026/02/03
//  Description   : 回復オブジェクトクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef HEAL_H_
#define HEAL_H_

#include "Engine/Core/Actor/Actor.h"

class Heal : public Actor {
public:
    Heal(IWorld* world, const GSvector3& position, const GSvector3& rotate, GSuint mesh, int heal_value);

    ~Heal() = default;

public:
    void update(float delta_time) override;

    void draw() const override;

public:
    void message(const std::string& message, std::any& param) override;

    bool can_interact() const override;

    std::string interact_text() const override;

private:
    bool is_open_{ false };

    GSuint mesh_{ 0 };

    int heal_value_{ 1 };
};


#endif
