#include "CinemaActorGenerator.h"
#include "Actor/CinemaActor/CinemaActor.h"

//#include "Actor/CinemaActor/CinemaBehavior/NormalEnemyCounter.h"
#include "Actor/CinemaActor/CinemaBehavior/PlayReact.h"

CinemaActorGenerator::CinemaActorGenerator(const json& j, World* world) {
    world_ = world;

    // jsonは使わない
}

Actor* CinemaActorGenerator::generate(const GSvector3& position, const GSvector3& rotate, int hp, int damage, const json& param) {
    CinemaActor* p = new CinemaActor{ world_, MyJson::get_string(param, "Timeline"), MyJson::get_boolean(param, "FixedPlayer", true)};

    // パラメータを追加
    const int type = MyJson::get_int(param, "Type");
    switch (type) {
        // リアクション発動
    case 0:
        p->set_position(position).set_rotation(rotate).set_collider(MyJson::get_float(param, "Radius"), 0.0f);
        p->set_behavior(new PlayReact{ world_, MyJson::get_string(param, "Target") });
        break;
    default: break;
    }

    world_->add_actor(p);
    return p;
}
