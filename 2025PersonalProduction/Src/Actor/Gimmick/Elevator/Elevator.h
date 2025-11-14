// -----------------------------------------------------------------------------------------
//  File          : Elevator.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/14
//  Updated       : 2025/11/14
//  Description   : エレベーターギミッククラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------


#ifndef ELEVATOR_H_
#define ELEVATOR_H_

#include "Engine/Core/Actor/FieldActor/FieldActor.h"

class Elevator : public FieldActor {
public:
    Elevator(IWorld* world, const GSvector3& position, const GSvector3& end_position, float time);

    ~Elevator() = default;

public:
    void update(float delta_time) override;

    void message(const std::string& message, std::any& param)override;

    bool can_interact() const override;

    std::string interact_text() const override;

    void die() override;

private:
    enum class State {
        Idle,
        Up,
        Down
    };

private:
    // 移動にかかる時間
    float move_time_{ 0 };
    // タイマー
    float timer_{ 0.0f };

    GSvector3 position_{ 0.0f, 0.0f, 0.0f };
    GSvector3 end_position_{ 0.0f, 0.0f, 0.0f };

    State state_{ State::Idle };
    State prev_state_{ State::Idle };

};


#endif
