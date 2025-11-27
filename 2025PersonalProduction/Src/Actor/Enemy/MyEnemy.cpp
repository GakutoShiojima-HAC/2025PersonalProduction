#include "MyEnemy.h"
#include "Engine/Utils/MyMath.h"
#include "Assets.h"
#include "Engine/Sound/SE.h"

MyEnemy::MyEnemy(IWorld* world, const GSvector3& position, const GSvector3& rotate, const MyEnemyInfo& info) :
    my_info_{ info },
    navmesh_{ this, world->navmesh() } {
    world_ = world;
    tag_ = ActorTag::Enemy;
    name_ = info.name;

    hp_ = info.hp;
    display_hp_ = (float)info.hp;

    init_parameter(PawnParameter::get_type(info.pawn_type));
    mesh_ = { info.skinmesh, info.skinmesh, info.skinmesh };

    navmesh_.offset_ratio() = 0.0f;

    transform_.position(position);
    transform_.eulerAngles(rotate);
    collide_field();
    mesh_.transform(transform_.localToWorldMatrix());
    origin_position_ = transform_.position();

    // モーションによるアラートイベントの追加
    for (const auto& data : info.attack_data) {
        set_motion_alert_event(data.first, data.second.start_time, data.second.bone);
    }
}

void MyEnemy::update(float delta_time) {
    update_invincible(delta_time);
    update_state(delta_time);
    update_gravity(delta_time);
    update_mesh(delta_time);
}

void MyEnemy::late_update(float delta_time) {
    update_display_hp(delta_time);
    update_alert_effect();
}

void MyEnemy::draw() const {
    mesh_.draw();

#ifdef _DEBUG
    navmesh_.draw_path();
#endif
}

void MyEnemy::draw_gui() const {
    draw_hp_gauge();
}

bool MyEnemy::is_attack_soon() const {
    auto it = my_info_.attack_data.find(motion_);
    if (it == my_info_.attack_data.end()) return false;

    // アラート開始から攻撃判定生成までを真とする
    return state_timer_ >= it->second.start_time && state_timer_ <= it->second.attack_time;
}

void MyEnemy::react(Actor& other) {
    if (other.tag() == ActorTag::Enemy) collide_actor(other);
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

bool MyEnemy::start_move() {
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

bool MyEnemy::is_attack_motion(GSuint motion) const {
    auto it = my_info_.attack_data.find(motion);
    if (it == my_info_.attack_data.end()) return false;
    return true;
}

void MyEnemy::set_motion_attack_event(GSuint motion, const MyEnemyAttackData& data) {
    mesh_.add_animation_event(motion, data.attack_time, [=] { generate_attack_collider(data.offset, data.radius, data.damage); });
}

void MyEnemy::generate_attack_collider(const GSvector3& offset, float radius, int damage) {
    GSmatrix4 m = local_to_world(offset, GSvector3::zero(), GSvector3::one());
    world_->generate_attack_collider(radius, m.position(), this, my_info_.damage + damage, name_ + "Attack", 0.1f, 0.0f);
}

void MyEnemy::draw_hp_gauge() const {
    // UIを描画するためにカメラを取得
    Camera* camera = world_->get_camera();
    if (camera == nullptr) return;
    // カメラ外なら終了
    Line line{ camera->transform().position(), collider().center };
    const GSvector3 to_target = line.start - line.end;
    if (GSvector3::dot(camera->transform().forward(), to_target) > 0) return;
    // 障害物があったら見えていないとして終了
    if (world_->get_field()->collide(line)) return;
    // 範囲の外なら見えていないとして終了
    const float length = to_target.magnitude();
    if (length > 30.0f) return;

    // スクリーン座標を計算
    const GSmatrix4 wmat = local_to_world(GSvector3{ 0.0f, my_info_.ui_height, 0.0f }, GSvector3::zero(), GSvector3::one());
    const GSvector3 world_position = wmat.position();
    GSvector2 screen_position;
    gsCalculateScreen(&screen_position, &world_position);

    // 背景を描画
    {
        const GSrect pic_rect{ 0.0f, 0.0f, 440.0f, 25.0f };
        const GSvector2 scale{ 0.25f, 0.25f };
        const GSvector2 position = screen_position - GSvector2{ pic_rect.right / 2.0f, pic_rect.bottom / 2.0f } *scale;

        Canvas::draw_texture(
            (GSuint)TextureID::HPGaugeBG,
            position,
            pic_rect,
            GSvector2{ 0.0f, 0.0f },
            scale
        );
    }

    // 体力を描画
    {
        GSrect pic_rect{ 0.0f, 0.0f, 434.0f, 21.0f };
        const GSvector2 scale{ 0.25f, 0.25f };
        const GSvector2 position = screen_position - GSvector2{ pic_rect.right / 2.0f, pic_rect.bottom / 2.0f } * scale;
        // 体力の割合
        const float ratio = display_hp_ / (float)my_info_.hp;
        pic_rect.right *= ratio;

        Canvas::draw_texture(
            (GSuint)TextureID::HPGauge,
            position,
            pic_rect,
            GSvector2{ 0.0f, 0.0f },
            scale,
            GScolor{ 0.772f, 0.192f, 0.192f, 1.0f },
            0.0f
        );
    }
}

void MyEnemy::draw_boss_bar() const {
    if (target_ == nullptr) return;

    // 背景を描画
    {
        const GSrect pic_rect{ 0.0f, 0.0f, 687.0f, 25.0f };
        const GSvector2 position{ 617.0f, 82.0f };

        Canvas::draw_texture(
            (GSuint)TextureID::HPBossGaugeBG,
            position,
            pic_rect,
            GSvector2{ 0.0f, 0.0f },
            GSvector2{ 1.0f, 1.0f }
        );
    }

    // 体力を描画
    {
        GSrect pic_rect{ 0.0f, 0.0f, 683.0f, 21.0f };
        const GSvector2 position{ 619.0f, 84.0f };
        // 体力の割合
        const float ratio = display_hp_ / (float)my_info_.hp;
        pic_rect.right *= ratio;

        Canvas::draw_texture(
            (GSuint)TextureID::HPBossGauge,
            position,
            pic_rect,
            GSvector2{ 0.0f, 0.0f },
            GSvector2{ 1.0f, 1.0f },
            GScolor{ 0.772f, 0.192f, 0.192f, 1.0f },
            0.0f
        );
    }
}

void MyEnemy::set_motion_alert_event(GSuint motion, float time, GSuint bone_num) {
    mesh_.add_animation_event(motion, time, [=] { play_alert_event(bone_num); });
}

void MyEnemy::play_alert_event(GSuint bone_num) {
    alert_effect_bone_num_ = bone_num;

    // エフェクトを再生
    const GSmatrix4 mat = mesh_.bone_matrices(bone_num);
    alert_effect_handle_ = gsPlayEffectEx((GSuint)EffectID::DangerSignal, &mat);
    // SEを再生
    SE::play((GSuint)SEID::Alert, transform_.position());
}

void MyEnemy::update_alert_effect() {
    if (!gsExistsEffect(alert_effect_handle_)) return;

    GSmatrix4 mat = mesh_.bone_matrices(alert_effect_bone_num_);
    gsSetEffectMatrix(alert_effect_handle_, &mat);
    const GSvector3 scale{ 0.2f, 0.2f, 0.2f };
    gsSetEffectScale(alert_effect_handle_, &scale);
}
