#include "ExplodeRoad.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include <GSeffect.h>
#include "GameConfig.h"
#include "Engine/Sound/SE.h"
#include "Engine/Utils/MyRandom.h"

constexpr float RADIUS{ 1.0f };

ExplodeRoad::ExplodeRoad(IWorld* world, const GSvector3& position, const GSvector3& velocity, Actor* owner, int damage, int time, float interval) {
	world_ = world;
	tag_ = ActorTag::Projectile;
    name_ = "ExplodeRoad";

    owner_ = owner;
    damage_ = damage;
    time_ = time;
    counter_ = 0;
    interval_ = interval;
    timer_ = interval;  // 最初は即出現

    use_gravity_ = false;

    velocity_ = velocity;
    transform_.position(position);
    transform_.rotation(GSquaternion::lookRotation(velocity));

	enable_collider_ = true;
}

void ExplodeRoad::update(float delta_time) {
    // 判定を生成
    if (timer_ >= interval_) {
        timer_ = 0.0f;
        ++counter_;

        world_->generate_attack_collider(RADIUS, transform_.position() + GSvector3{ 0.0f, RADIUS, 0.0f }, owner_, damage_, "Attack", 0.1f, 0.0f,
            MyRandom::random_vec3(GSvector3{ -0.05f, 0.15f, -0.05f }, GSvector3{ 0.05f, 0.2f, 0.05f }));
        play_effect((GSuint)EffectID::ExplosionSmall, GSvector3::zero());
        world_->camera_shake(CameraShakeType::HandShake, 0.5f, 15.0f, false);
        SE::play_random((GSuint)SEID::Explosion, transform_.position(), 0.125f);

        // 終了か？
        if (counter_ >= time_) {
            die();
            return;
        }

        // 次の位置へ
        transform_.translate(velocity_, GStransform::Space::World);
    }
    else {
        timer_ += delta_time / cFPS;
    }
    
}
