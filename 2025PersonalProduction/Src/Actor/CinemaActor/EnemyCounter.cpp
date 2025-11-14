#include "Actor/CinemaActor/EnemyCounter.h"
#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "State/Player/PlayerState.h"
#include "Assets.h"

EnemyCounter::EnemyCounter(IWorld* world, const GSvector3& position, int enemy_count) {
    world_ = world;
    name_ = "EnemyCounter";
    tag_ = ActorTag::None;
    enable_collider_ = false;
    enable_timescale_ = false;

    mesh_ = (GSuint)MeshID::GimmickDoor;
    mesh_collider_ = (GSuint)MeshID::GimmickDoor;

    transform_.position(position);

    int count{ 0 };
    for (const auto& actor : world_->get_all_actor()) {
        if (actor->tag() == ActorTag::Enemy) ++count;
    }
    counter_ = count - enemy_count;
}

void EnemyCounter::update(float delta_time) {
    if (state_ == State::Idle) {
        // エネミーの数を取得
        int count{ 0 };
        for (const auto& actor : world_->get_all_actor()) {
            if (actor->tag() == ActorTag::Enemy) ++count;
        }

        // 通常エネミーが残っていないと判断出来たら
        if (count <= counter_) {
            // タイムラインを再生する
            state_ = State::Event;
            world_->play_timeline("OpenDoor");
            // プレイヤーの動きを止めておく
            Character* player = world_->find_character("Player");
            if (player != nullptr) {
                player_prev_state_ = player->current_state_num();
                player->change_state((GSuint)PlayerStateType::Idle);
            }
            return;
        }
    }
    else {
        // タイムライン再生が終わったら
        if (!world_->is_playing_timeline()) {
            // プレイヤーを戻す
            Character* player = world_->find_character("Player");
            if (player != nullptr) {
                player->change_state(player_prev_state_);
            }
            // 終了(死亡)
            is_dead_ = true;
            return;
        }
    }
}

void EnemyCounter::draw_gui() const {
    if (state_ != State::Idle) return;

    // TODO 残りの敵の数を描画する
}

void EnemyCounter::die() {
    // 外部から削除させない
}
