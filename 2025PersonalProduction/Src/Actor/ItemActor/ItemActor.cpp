#include "Actor/ItemActor/ItemActor.h"
#include "Engine/Core/World/IWorld.h"
#include "Item/ItemDataManager.h"
#include "Assets.h"
#include "Engine/Utils/MyString.h"

ItemActor::ItemActor(IWorld* world, const GSvector3& position) {
    world_ = world;
    transform_.position(position);
    init();
}

ItemActor::ItemActor(IWorld* world, const GSvector3& position, ItemType type, GSuint id) {
    world_ = world;
    transform_.position(position);
    data_.type = type;
    data_.id = id;
    init();
}

ItemActor::ItemActor(IWorld* world, const GSvector3& position, ItemData::Data data) {
    world_ = world;
    transform_.position(position);
    data_ = data;
    init();
}

ItemActor::~ItemActor() {

}

void ItemActor::update(float delta_time) {
    // インタラクト済みなら死亡
    if (is_interact_) {
        die();
        return;
    }
}

void ItemActor::draw() const {
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh((GSuint)MeshID::Item);
    glPopMatrix();
}

void ItemActor::die() {
    is_dead_ = true;
}

void ItemActor::message(const std::string& message, std::any& param) {
    if (!is_interact_ && message == "PlayerInteract") {
        world_->game_save_data().inventory().add(data_, 1);
        is_interact_ = true;
    }
}

bool ItemActor::can_interact() const {
    return true;
}

std::string ItemActor::interact_text() const {
    return MyString::utf8_to_shiftjis(interact_text_);
}

void ItemActor::init() {
    interact_text_ = ItemDataManager::get_instance().get_name(data_);
    tag_ = ActorTag::Item;
    name_ = interact_text_;
    enable_collider_ = false;
}

