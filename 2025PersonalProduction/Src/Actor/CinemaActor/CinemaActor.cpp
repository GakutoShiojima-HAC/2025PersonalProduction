#include "Actor/CinemaActor/CinemaActor.h"
#include "Actor/CinemaActor/CinemaBehavior/ICinemaBehavior.h"
#include "Engine/Core/World/IWorld.h"
#include "State/Player/PlayerState.h"

CinemaActor::CinemaActor(IWorld* world, const std::string& timeline_name, bool is_playing_fixed_player) {
    world_ = world;
    tag_ = ActorTag::None;
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
                player->change_state(player_prev_state_);
            }
            end();
        }
        return;
    }

    // 再生条件を満たしたら
    if (behavior_->is_start()) {
        play_start_ = true;
        world_->play_timeline(timeline_name_);
        behavior_->enter();

        // プレイヤーを固定するかどうか
        if (is_player_fixed_) {
            Character* player = world_->find_character("Player");
            if (player != nullptr) {
                player_prev_state_ = player->current_state_num();
                player->change_state((GSuint)PlayerStateType::Idle);
            }
            return;
        }
        // 固定しないならこの後存在する意味がないので終了
        else {
            end();
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
