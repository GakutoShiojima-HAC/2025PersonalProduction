#ifndef STAGE_TEREPORTER_H_
#define STAGE_TEREPORTER_H_

#include "Engine/Core/Actor/Actor.h"

class StageTereporter : public Actor {
public:
    StageTereporter(IWorld* world, const GSvector3& position, const GSvector3& rotate, int stage_id);

    ~StageTereporter() = default;

public:
    void react(Actor& other) override;

    void die() override;

private:
    int stage_id_{ 0 };

};


#endif
