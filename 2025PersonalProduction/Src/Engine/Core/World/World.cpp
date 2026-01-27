#include "Engine/Core/World/World.h"
#include <GSeffect.h>
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Collision/AttackCollider.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Sound/SE.h"

World::World() {
    avoid_effect_ = AvoidEffectManager{ this };
}

World::~World() {
	clear();
}

void World::update(float delta_time) {
    if (impact_effect_.is_active()) {
        impact_effect_.update(delta_time);
        return;
    }

    const float scale_time = delta_time * timescale_;

    game_timer_.update(delta_time);

	field_->update(delta_time, scale_time);
	if (navmesh_ != nullptr) navmesh_->update(scale_time);
	actor_.update(delta_time, scale_time);
	actor_.collide();
	timeline_.update(delta_time, scale_time);
	camera_.update(delta_time);
	actor_.late_update(delta_time, scale_time);
	gsUpdateEffect(scale_time); // ハンドル指定で計算で戻せる gsSetEffectSpeed

	if (attack_collider_pool_ != nullptr) attack_collider_pool_->remove();
	character_.remove();
	pawn_.remove();
	actor_.remove();

    avoid_effect_.update(delta_time);
    action_score_.update(delta_time);
    type_writer_label_.update(delta_time);

    if(camera_.current() != nullptr) SE::listener(camera_.current()->transform());
}

void World::draw() const {
    // ポストエフェクトの開始
    game_post_effect_.begin();

	camera_.draw();
	gsSetEffectCamera();
	light_->draw();
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

	field_->draw();
	if (navmesh_ != nullptr) navmesh_->draw();
	actor_.draw();
	actor_.draw_tranparent();
    gsDrawEffect();

    // ポストエフェクトの終了
    game_post_effect_.end();

	// マスクエフェクトの描画
    if (game_post_effect_.is_draw_mask()) {
        game_post_effect_.begin_mask();

        // 回避エフェクトか
        if (game_post_effect_.is_draw_avoid_effect()) {
            actor_.draw();
            actor_.draw_tranparent();
            gsDrawEffect();
        }

        game_post_effect_.end_mask();
    }
	
    // ポストエフェクトを適用
    GSuint current = game_post_effect_.apply(camera_.get_projection_matrix());

    // GUIの描画
    if (enable_draw_gui_) {
        game_post_effect_.begin_gui(current);

        actor_.draw_gui();
        if (enable_draw_game_info_gui_) {
            action_score_.draw();
            game_timer_.draw();
            type_writer_label_.draw();
        }

        game_post_effect_.end_gui();
    }

    current = game_post_effect_.apply_dissolve(current);

    // 最終結果を描画
    game_post_effect_.draw(current);
}

void World::clear() {
    impact_effect_.clear();
    avoid_effect_.clear();
    static_effect_.clear();
	delete field_;
	field_ = nullptr;
	delete light_;
	light_ = nullptr;
	delete navmesh_;
	navmesh_ = nullptr;
	delete attack_collider_pool_;
	attack_collider_pool_ = nullptr;
	character_.clear();
	pawn_.clear();
	timeline_.clear();
	actor_.clear();
	camera_.clear();
    player_respawner_.clear();
    type_writer_label_.clear();
}

void World::shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection) {
	World* self = (World*)param;
	// シャドウマップにアクターを描画
	self->actor_.draw();
	// シャドウマップにフィールド用のアクターを描画
	self->field_->draw_field_actor();
	// シャドウマップにフィールド全体を描画（フィード用のアクターも含む）
	//self->field_->draw(); // これ使う場合はフィールド用のアクター描画をコメントアウト
}

void World::add_field(Field* field) {
	delete field_;
	field_ = field;
}

void World::add_light(Light* light) {
	delete light_;
	light_ = light;
}

void World::add_navmesh(NavMeshSurface* navmesh) {
	delete navmesh_;
	navmesh_ = navmesh;
}

void World::generate_static_effect() {
    static_effect_.generate();
}

void World::skip_type_writer() {
    type_writer_label_.skip();
}

void World::set_type_writer(const std::string& text, TextCode code) {
    type_writer_label_.set(text, code);
}

void World::set_type_writer(const std::vector<std::string>& text, TextCode code) {
    type_writer_label_.set(text, code);
}

void World::impact_effect_start(float time) {
    impact_effect_.start(time);
}

bool& World::enable_mask_avoid_effect() {
    return game_post_effect_.enable_draw_avoid_effect();
}

void World::add_actor(Actor* actor) {
	actor_.add(actor);
}

void World::add_pawn(Pawn* pawn) {
	pawn_.add(pawn);
	actor_.add(pawn);
}

void World::add_character(Character* character) {
	character_.add(character);
	add_pawn(character);
}

void World::add_attack_collider_pool(AttackColliderPool* pool) {
	delete attack_collider_pool_;
	attack_collider_pool_ = pool;
}

GameTimer& World::time() {
    return game_timer_;
}

Timeline& World::timeline() {
    return timeline_;
}

PlayerRespawner& World::player_respawner() {
    return player_respawner_;
}

void World::load_static_effect(const std::string& json_file) {
    static_effect_.load(json_file);
}

bool& World::enable_draw_gui() {
    return enable_draw_gui_;
}

bool& World::enable_draw_game_info_gui() {
    return enable_draw_game_info_gui_;
}

Field* World::get_field() {
	return field_;
}

Light* World::get_light() {
	return light_;
}

void World::add_camera(Camera* camera) {
	camera_.add(camera);
}

Camera* World::get_camera() const {
	return camera_.current();
}

Camera* World::find_camera(const CameraTag tag) const {
	return camera_.find(tag);
}

void World::camera_transition(Camera* to, float time) {
	camera_.transition(to, time);
}

void World::camera_transition(Camera* from, Camera* to, float time) {
	camera_.transition(from, to, time);
}

void World::camera_shake(CameraShakeType type, float duration, float strength, bool loop) {
    camera_.shake(type, duration, strength, loop);
}

void World::camera_shake_end() {
    camera_.shake_end();
}

bool& World::camera_enable_shake() {
    return camera_.enable_shake();
}

int World::camera_effect_play_foward(GSuint id, float time) {
	return camera_.play_effect(id, time);
}

NavMeshSurface* World::navmesh() {
	return navmesh_;
}

std::vector<Actor*> World::get_all_actor() const {
	return actor_.get_all();
}

Actor* World::find_actor(const string& name) const {
	return actor_.find(name);
}

vector<Actor*> World::find_actor_with_tag(const ActorTag tag) const {
	return actor_.find_with_tag(tag);
}

int World::count_actor() const {
	return actor_.count();
}

int World::count_actor_with_tag(const ActorTag tag) const {
	return actor_.count_with_tag(tag);
}

Pawn* World::find_pawn(const string& name) const {
	return pawn_.find(name);
}

vector<Pawn*> World::find_pawn_with_tag(const ActorTag tag) const {
	return pawn_.find_with_tag(tag);
}

Character* World::find_character(const string& name) const {
	return character_.find(name);
}

vector<Character*> World::find_character_with_tag(const ActorTag tag) const {
	return character_.find_with_tag(tag);
}

void World::play_timeline(const std::string& name, bool enable_timescale) {
    timeline_.play(name, enable_timescale);
}

void World::stop_timeline() {
    timeline_.stop();
}

bool World::is_playing_timeline() const {
	return timeline_.is_playing();
}

void World::generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan, float delay, const GSvector3& external_velocity) {
	// オブジェクトプールがあるならプール管理する
	if (attack_collider_pool_ != nullptr) attack_collider_pool_->generate(radius, center, owner, damage, name, lifespan, delay, external_velocity);
	else add_actor(new AttackCollider{ radius, center, owner, damage, name, lifespan, delay, external_velocity });
}

void World::set_mask_color(const GScolor& color) {
    game_post_effect_.set_mask_color(color);
}

bool World::is_draw_mask() const {
    return game_post_effect_.is_draw_mask();
}

void World::start_avoid_effect(float time, float time_scale) {
    avoid_effect_.start(time, time_scale);
}

void World::pause_avoid_effect(float time) {
    avoid_effect_.pause(time);
}

void World::resume_avoid_effect() {
    avoid_effect_.resume();
}

bool World::is_avoid_effect() const {
    return avoid_effect_.is_effect();
}

float& World::set_blur_effect_power() {
    return game_post_effect_.blur_power();
}

GameSaveData& World::game_save_data() {
    return game_save_data_;
}

ActionScore& World::action_score() {
    return action_score_;
}

float& World::timescale() {
    return timescale_;
}

void World::set_timescale(float scale, float time) {
    Tween::cancel("WorldTimeScale");
    if (time <= 0.0f) timescale_ = scale;
    else Tween::value(timescale_, scale, time, [&](float current) { timescale() = current; }).name("WorldTimeScale").ease(EaseType::EaseOutQuad);
}

void World::update_check_point(const GSvector3& position, const GSvector3& rotate) {
    player_respawner_.add_point(position, rotate);
}
