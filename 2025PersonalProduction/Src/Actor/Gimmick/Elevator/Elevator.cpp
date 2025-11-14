#include "Actor/Gimmick/Elevator/Elevator.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include <gslib.h>
#include "GameConfig.h"

Elevator::Elevator(IWorld* world, const GSvector3& position, const GSvector3& end_position, float time) {
    world_ = world;
    tag_ = ActorTag::Object;
    name_ = "Elevator";

    transform_.position(position);

    position_ = position;
    end_position_ = end_position;

    enable_collider_ = true;

    mesh_ = (GSuint)MeshID::GimmickElevator;
    mesh_collider_ = (GSuint)MeshID::GimmickElevator;

    move_time_ = time;

    state_ == State::Idle;
}

void Elevator::update(float delta_time) {
    if (state_ == State::Idle) return;

    // 進捗率計算
    const float progress = timer_ / move_time_;
    // タイマー加算
    timer_ += delta_time / cFPS;

    auto move = [&](const GSvector3& start, const GSvector3& end) {
        const GSvector3 position = GSvector3::lerp(start, end, progress);
        transform_.position(position);
    };

    // 移動
    if (state_ == State::Up) {
        move(position_, end_position_);
    }
    else {
        move(end_position_, position_);
    }

    // 終了
    if (progress >= 1.0f) {
        prev_state_ = state_;   // 前の移動方向を保存
        state_ = State::Idle;
    }
}

void Elevator::message(const std::string& message, std::any& param) {
    if (message == "PlayerInteract") {
        // 移動中
        if (state_ != State::Idle) return;
        // 上へ
        if (prev_state_ != State::Up) {
            timer_ = 0.0f;
            state_ = State::Up;
        }
        // 下へ
        if (prev_state_ == State::Up) {
            timer_ = 0.0f;
            state_ = State::Down;
        }
    }
}

bool Elevator::can_interact() const {
    return state_ == State::Idle;
}

std::string Elevator::interact_text() const {
    return "エレベーター";
}

void Elevator::die() {
    // 外部から削除させない
}
