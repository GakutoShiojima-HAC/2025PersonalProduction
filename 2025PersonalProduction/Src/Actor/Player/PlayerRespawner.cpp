#include "PlayerRespawner.h"

PlayerRespawner::~PlayerRespawner() {
    clear();
}

void PlayerRespawner::clear() {
    can_respawn_ = false;
}

void PlayerRespawner::add_point(const GSvector3& position, const GSvector3& rotate) {
    can_respawn_ = true;
    respawn_position_ = position;
    respawn_rotate_ = rotate;
}

bool PlayerRespawner::can_respawn() const {
    return can_respawn_;
}

GSvector3 PlayerRespawner::respawn_position() const {
    return respawn_position_;
}

GSvector3 PlayerRespawner::respawn_rotate() const {
    return respawn_rotate_;
}
