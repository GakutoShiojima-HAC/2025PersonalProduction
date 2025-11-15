// -----------------------------------------------------------------------------------------
//  File        : Barrier.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/11/15
//  Updated     : 2025/11/15
//  Description : ボス部屋への障壁となるフィールドアクタークラス  
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef BARRIER_H_
#define BARRIER_H_

#include "Engine/Core/Actor/FieldActor/FieldActor.h"

class Barrier : public FieldActor {
public:
    Barrier(IWorld* world, const GSvector3& position, const std::string& break_type, float break_time, float parameter);

public:
    void update(float delta_time) override;

    void message(const std::string& message, std::any& param) override;

private:
    enum class BarrierBreakType {
        None,
        Down
    };

private:
    BarrierBreakType break_type_{ BarrierBreakType::None };

    bool is_break_{ false };

    float timer_{ 0.0f };

    float parameter_{ 0.0f };
};


#endif
