#ifndef STAGE_TEREPORTER_H_
#define STAGE_TEREPORTER_H_

#include "Engine/Core/Actor/Actor.h"

class StageTereporter : public Actor {
public:
    StageTereporter(IWorld* world, const GSvector3& position, const GSvector3& rotate, int stage_id);

    ~StageTereporter() = default;

public:
    void update(float delta_time) override;

    void late_update(float delta_time) override;

    void draw() const override;

    void react(Actor& other) override;

    void die() override;

private:
    int stage_id_{ 0 };

    float timer_{ 0.0f };

    bool is_touch_{ false };

};


#endif
