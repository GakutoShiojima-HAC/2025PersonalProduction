#ifndef PLAYER_AVOID_COLLIDER_H_
#define PLAYER_AVOID_COLLIDER_H_

#include "Engine/Core/Actor/Pawn/Pawn.h"

class Player;

class PlayerAvoidCollider : public Pawn {
public:
    PlayerAvoidCollider(IWorld* world, Player& owner);

public:
    void take_damage(Actor& other, const int damage) override;

public:
    /// <summary>
    /// コライダーの切り替え
    /// </summary>
    /// <returns>参照</returns>
    bool& enabled();

private:
    void start_effect();

private:
    Player& owner_;

};

#endif
