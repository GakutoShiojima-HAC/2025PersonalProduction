#include "Engine/Core/World/World.h"
#include <GSeffect.h>
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"

World::~World() {
	clear();
}

void World::update(float delta_time) {
	field_->update(delta_time);
	navmesh_->update(delta_time);
	actor_.update(delta_time);
	actor_.collide();
	actor_.late_update(delta_time);
	timeline_.update(delta_time);
	camera_.update(delta_time);
	gsUpdateEffect(delta_time);
	actor_.remove();
}

void World::draw() const {
	camera_.draw();
	gsSetEffectCamera();
	light_->draw();
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);
	field_->draw();
	navmesh_->draw();
	actor_.draw();
	actor_.draw_tranparent();
	gsDrawEffect();
	actor_.draw_gui();
}

void World::clear() {
	delete field_;
	field_ = nullptr;
	delete light_;
	light_ = nullptr;
	delete navmesh_;
	navmesh_ = nullptr;
	actor_.clear();
	camera_.clear();
	timeline_.clear();
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

void World::add_actor(Actor* actor) {
	actor_.add(actor);
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

NavMeshSurface* World::navmesh() {
	return navmesh_;
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

TimelineManager& World::timeline() {
	return timeline_;
}
