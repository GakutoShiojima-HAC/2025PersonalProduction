#include "State/Player/PlayerFallState.h"
#include "Actor/Player/Player.h"

PlayerFallState::PlayerFallState(Player& owner) :
	PlayerState(owner) {

}

void PlayerFallState::enter() {

}

void PlayerFallState::update(float delta_time) {
	owner_.update_move_air(delta_time);
}

void PlayerFallState::draw() const {

}

void PlayerFallState::draw_gui() const {

}

void PlayerFallState::exit() {

}
