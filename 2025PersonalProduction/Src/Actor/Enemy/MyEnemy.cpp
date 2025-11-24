#include "MyEnemy.h"
#include "Engine/Utils/MyMath.h"

MyEnemy::MyEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& info) :
    my_info_{ info } {
    world_ = world;
    tag_ = ActorTag::Enemy;
    name_ = info.name;

    hp_ = info.hp;

    init_parameter(PawnParameter::get_type(info.type));
    mesh_ = { info.skinmesh, info.skinmesh, info.skinmesh };

    // ナビメッシュ追加
    navmesh_ = { this, world_->navmesh() };
    navmesh_.offset_ratio() = 0.0f;

    transform_.position(position);
    transform_.eulerAngles(rotate);
    mesh_.transform(transform_.localToWorldMatrix());
    collide_field();
    origin_position_ = transform_.position();
}

void MyEnemy::update(float delta_time) {
    update_invincible(delta_time);
    update_state(delta_time);
    update_gravity(delta_time);
    update_mesh(delta_time);
}

void MyEnemy::late_update(float delta_time) {
    update_denger_signal(delta_time);
}

void MyEnemy::draw() const {
    mesh_.draw();

#ifdef _DEBUG
    navmesh_.draw_path();
#endif
}

void MyEnemy::react(Actor& other) {
    if (other.tag() == ActorTag::Enemy) collide_actor(other);
}

void MyEnemy::update_mesh(float delta_time) {
    // メッシュのモーションを更新
    mesh_.update(delta_time);
    // ルートモーションを適用
    if (is_root_motion_state()) mesh_.apply_root_motion(transform_);
    // ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}

void MyEnemy::save_current_state() {
    prev_state_num_ = state_.get_current_state();
    prev_motion_num_ = motion_;
    prev_motion_loop_ = motion_loop_;
}

void MyEnemy::change_prev_state() {
    change_state(prev_state_num_, prev_motion_num_, prev_motion_loop_);
}

GSuint MyEnemy::prev_state_num() const {
    return prev_state_num_;
}

bool MyEnemy::search_target() {
    if (target_ != nullptr) {
        // 死亡判定なら再検索
        if (target_->is_dead_state()) target_ = nullptr;
        // 存在しているなら真
        else return true;
    }
    std::vector<Character*> character = world_->find_character_with_tag(ActorTag::Player);
    if (character.empty()) {
        target_ = nullptr;
        return false;
    }
    // 一番近いキャラクターを探す
    float min_length{ FLT_MAX };
    Character* target{ nullptr };
    for (const auto& chara : character) {
        // 死亡していたらスキップ
        if (chara->is_dead_state()) continue;
        // 自分の視野の外なら見えていないとしてスキップ
        if (MyMath::to_target_angle(transform_.position(), transform_.forward(), chara->transform().position()) > my_info_.search_fov) continue;
        // 障害物があったら見えていないとしてスキップ
        Line line{ collider().center, chara->collider().center };
        if (world_->get_field()->collide(line)) continue;
        // 索敵範囲の外なら見えていないとしてスキップ
        float length = (line.end - line.start).magnitude();
        if (length > my_info_.search_length) continue;
        // 一番近ければ対象を更新
        if (length < min_length) {
            min_length = length;
            target = chara;
        }
    }

    target_ = target;
    return target_ != nullptr;
}

Character* MyEnemy::target() {
    return target_;
}

bool MyEnemy::start_move()
{
    if (target_ == nullptr) return false;
    return navmesh_.find_path(target_);
}

bool MyEnemy::start_move(const GSvector3& to) {
    return navmesh_.find_path(to);
}

void MyEnemy::update_move(float delta_time) {
    navmesh_.update_move(delta_time, my_info_.move_speed, 3.0f);
}

bool MyEnemy::is_move_end() const {
    return navmesh_.is_end_move();
}

void MyEnemy::move_end() {
    navmesh_.end();
}

void MyEnemy::update_look_target(float delta_time, float angle) {
    if (target_ == nullptr) return;

    GSvector3 to_target = target()->transform().position() - transform_.position();
    GSquaternion rotation = GSquaternion::rotateTowards(
        transform_.rotation(),
        GSquaternion::lookRotation(to_target),
        angle * delta_time
    );
    transform_.rotation(rotation);
}

void MyEnemy::look_target() {
    if (target_ == nullptr) return;

    GSvector3 target_position = target_->transform().position();
    target_position.y = transform_.position().y; // 向かせるだけなのでy座標は同じ
    transform_.lookAt(target_position);
}

void MyEnemy::release_target() {
    target_ = nullptr;
}

GSvector3& MyEnemy::origin_position() {
    return origin_position_;
}

const MyEnemyInfo& MyEnemy::my_info() const {
    return my_info_;
}

void MyEnemy::generate_attack_collider() {
    GSmatrix4 m = local_to_world(my_info_.attack_offset, GSvector3::zero(), GSvector3::one());
    world_->generate_attack_collider(my_info_.attack_radius, m.position(), this, my_info_.attack_damage, name_ + "Attack", 0.1f, 0.0f);

}
