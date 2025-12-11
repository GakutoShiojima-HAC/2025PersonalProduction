#include "TreasureChest.h"
#include "Engine/Core/World/IWorld.h"
#include "Assets.h"
#include "Actor/ItemActor/ItemActor.h"

TreasureChest::TreasureChest(IWorld* world, const GSvector3& position, const GSvector3& rotate, ItemData::Data& data) {
	world_ = world;
	tag_ = ActorTag::Object;
    name_ = "TreasureChest";

	transform_.position(position);
    transform_.eulerAngles(rotate);

	enable_collider_ = true;
    collider_ = BoundingSphere{ 0.125f };

    generate_data_ = data;
}

void TreasureChest::update(float delta_time) {
    if (is_open_) die();
}

void TreasureChest::draw() const {
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh((GSuint)MeshID::TreasureChest);
    glPopMatrix();
}

void TreasureChest::message(const std::string& message, std::any& param) {
    if (message == "PlayerInteract" && !is_dead_) {
        open();
    }
}

bool TreasureChest::can_interact() const {
    return !is_open_;
}

std::string TreasureChest::interact_text() const {
    return "•ó” ";
}

void TreasureChest::open() {
    is_open_ = true;
    world_->add_actor(new ItemActor{ world_, transform_.position(), generate_data_ });
    int handle = play_effect((GSuint)EffectID::LootBox, GSvector3::zero());
    const GSvector3 scale = GSvector3::one() * 0.5f;
    gsSetEffectScale(handle, &scale);
}
