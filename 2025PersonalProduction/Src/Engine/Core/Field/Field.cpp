#include "Engine/Core/Field/Field.h"
#include "Engine/Core/Actor/FieldActor/FieldActor.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/Ray.h"
#include "Engine/Core/Collision/BoundingSphere.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

Field::Field(GSuint octree, GSuint collider, GSuint skybox) {
	octree_ = octree;
	collider_ = collider;
	skybox_ = skybox;
}

Field::~Field() {
	clear();
}

void Field::update(float delta_time, float scale_time) {
	actors_.update(delta_time, scale_time);
	actors_.remove();

#ifdef _DEBUG
    ImGui::Begin("Field Window");

    if (ImGui::Button("Toggle Draw Octree")) draw_octree_ = !draw_octree_;

    if (ImGui::Button("Toggle Draw Skybox")) draw_skybox_ = !draw_skybox_;

    ImGui::End();
#endif
}

void Field::draw() const {
#ifdef _DEBUG
    if (draw_skybox_) gsDrawSkyboxCubemap(skybox_);
    if (draw_octree_) gsDrawOctree(octree_);
#else
    gsDrawSkyboxCubemap(skybox_);
    gsDrawOctree(octree_);
#endif

	draw_field_actor();
}

void Field::clear() {
	actors_.clear();
}

void Field::draw_field_actor() const {
	actors_.draw();
}

void Field::draw_field_actor_shadow() const {
    actors_.draw_shadow();
}

void Field::add(FieldActor* field_actor) {
	actors_.add(field_actor);
}

Actor* Field::find(const std::string& name) {
    return actors_.find(name);
}

bool Field::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane, Actor** actor) const {
	Line line;
	line.start = ray.position;
	line.end = ray.position + (ray.direction.normalized() * max_distance);
	return collide(line, intersect, plane, actor);
}

bool Field::collide(const Line& line, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    // オクトリーとの交差判定
    GSvector3 octree_intersection_point;
    GSplane octree_intersection_plane;
    bool intersection_octree = gsOctreeCollisionLine(
        gsGetOctree(collider_),
        &line.start, &line.end, &octree_intersection_point, &octree_intersection_plane) == GS_TRUE;
    // アクターとの交差判定
    GSvector3 actor_intersection_point;
    GSplane actor_intersection_plane;
    Actor* intersection_actor = actors_.collide(line, &actor_intersection_point, &actor_intersection_plane);
    // オクトリーとアクター両方に衝突しているか？
    if (intersection_octree && intersection_actor != nullptr) {
        // 始点と交点の距離を計算する
        float distance_to_actor = line.start.distance(actor_intersection_point);
        float distance_to_octree = line.start.distance(octree_intersection_point);
        if (distance_to_octree < distance_to_actor) {
            // オクトリーの方が近い場合はアクターに衝突していないことにする
            intersection_actor = nullptr;
        }
        else {
            // アクターの方が近い場合オクトリーに衝突していないことにする
            intersection_octree = false;
        }
    }
    // オクトリーに衝突しているか？
    if (intersection_octree) {
        if (intersect != nullptr) {
            *intersect = octree_intersection_point;
        }
        if (plane != nullptr) {
            *plane = octree_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = nullptr;
        }
        return true;
    }
    // アクターに衝突しているか？
    if (intersection_actor) {
        if (intersect != nullptr) {
            *intersect = actor_intersection_point;
        }
        if (plane != nullptr) {
            *plane = actor_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = intersection_actor;
        }
        return true;
    }
    // 何も衝突しなかった
    return false;
}

bool Field::collide(const BoundingSphere& sphere, GSvector3* center, Actor** actor) const {
    // 衝突判定後の半径
    GSvector3 collided_center = sphere.center;
    // フィールド用のアクターとの衝突判定を行う
    Actor* collided_actor = actors_.collide(sphere, &collided_center);
    // オクトリーと衝突判定を行う
    bool collided_octree = gsOctreeCollisionSphere(
        gsGetOctree(collider_),
        &collided_center, sphere.radius, &collided_center) == GS_TRUE;
    if (center != nullptr) {
        *center = collided_center;
    }
    if (actor != nullptr) {
        *actor = collided_actor;
    }
    // オクトリーかアクターどちらかに衝突していたか？
    return collided_octree || collided_actor != nullptr;
}

GSuint& Field::octree() {
    return octree_;
}

GSuint& Field::collider() {
    return collider_;
}

GSuint& Field::skybox() {
    return skybox_;
}

std::vector<Actor*> Field::get_all_actor() {
    return actors_.get_all();
}
