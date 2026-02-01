#include "Actor/CinemaActor/CinemaActor.h"
#include "Actor/CinemaActor/CinemaBehavior/ICinemaBehavior.h"
#include "Engine/Core/World/IWorld.h"
#include "Actor/Player/Player.h"
#include "State/Player/PlayerState.h"

CinemaActor::CinemaActor(IWorld* world, const std::string& timeline_name, bool is_playing_fixed_player) {
    world_ = world;
    tag_ = ActorTag::Object;
    name_ = "CinemaActor";

    timeline_name_ = timeline_name;
    is_player_fixed_ = is_playing_fixed_player;
    enable_timescale_ = false;
    enable_collider_ = false;
}

CinemaActor::~CinemaActor() {
    end();
}

void CinemaActor::update(float delta_time) {
    // 行動がなければ終了
    if (behavior_ == nullptr) {
        end();
        return;
    }

    behavior_->update(delta_time);

    // 再生中
    if (play_start_) {
        // 再生が終わったらプレイヤーのステートを戻して終了
        if (!world_->is_playing_timeline()) {
            Character* player = world_->find_character("Player");
            if (player != nullptr) {
                player->velocity() = GSvector3{ 0.0f, 0.0f, 0.0f };
                player->change_state((GSuint)PlayerStateType::Move, PlayerMotion::Idle, true);
            }
            world_->enable_draw_gui() = true;
            end();
        }
        return;
    }

    // 再生条件を満たしたら
    if (behavior_->is_start()) {
        play_start_ = true;
        world_->stop_timeline();
        world_->play_timeline(timeline_name_);
        behavior_->enter();
        world_->enable_draw_gui() = false;

        // プレイヤーを固定するかどうか
        if (is_player_fixed_) {
            Character* player = world_->find_character("Player");
            if (player != nullptr) {
                player->change_state((GSuint)PlayerStateType::Idle, PlayerMotion::Idle, true);
            }
            return;
        }
    }
}

void CinemaActor::draw() const {
    behavior_->draw();
}

void CinemaActor::draw_gui() const {
    behavior_->draw_gui();
}

void CinemaActor::die() {
    // 外部から削除させない
}

void CinemaActor::react(Actor& other) {
    behavior_->react(other);
}

CinemaActor& CinemaActor::set_position(const GSvector3& position) {
    transform_.position(position);
    return *this;
}

CinemaActor& CinemaActor::set_rotation(const GSvector3& rotate) {
    transform_.eulerAngles(rotate);
    return *this;
}

CinemaActor& CinemaActor::set_collider(float radius, float height) {
    if (radius <= 0.0f) return *this;

    collider_ = BoundingSphere{ radius, GSvector3{ 0.0f, height, 0.0f } };
    enable_collider_ = true;
    return *this;
}

CinemaActor& CinemaActor::set_behavior(ICinemaBehavior* behavior) {
    delete behavior_;
    behavior_ = behavior;
    return *this;
}

void CinemaActor::end() {
    delete behavior_;
    behavior_ = nullptr;
    enable_collider_ = false;
    is_dead_ = true;
}
