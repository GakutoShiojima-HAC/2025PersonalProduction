#include "Engine/Core/Actor/Actor.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Utils/Line.h"
#include "GameConfig.h"
#include <GSeffect.h>

constexpr float FOOT_OFFSET{ 0.125f };

void Actor::update(float delta_time) {
    //update_physics(delta_time);
    //collide_field();
}

void Actor::late_update(float delta_time) {

}

void Actor::draw() const {

}

void Actor::draw_transparent() const {

}

void Actor::draw_gui() const {

}

#ifdef _DEBUG
void Actor::debug_update(float delta_time) {
    // 名前
    std::string text = "name: " + name_;
    ImGui::Text(text.c_str());
    // 座標
    ImGui::Text("position: X:%.3f Y:%.3f Z:%.3f", transform_.position().x, transform_.position().y, transform_.position().z);
}
#endif

void Actor::die() {
	is_dead_ = true;
}

void Actor::react(Actor& other) {

}

void Actor::collide(Actor& other) {
	// どちらのアクターも衝突判定が有効なら
	if (enable_collider_ && other.enable_collider_) {
		// 衝突判定を行う
		if (is_collide(other)) {
			// 衝突した場合は衝突リアクションを行う
			react(other);
			other.react(*this);
		}
	}
}

bool Actor::is_dead() const {
	return is_dead_;
}

bool Actor::is_clear() const {
	return is_clear_;
}

bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

string& Actor::name() {
	return name_;
}

ActorTag& Actor::tag() {
	return tag_;
}

const GStransform& Actor::transform() const {
	return transform_;
}

GStransform& Actor::transform() {
	return transform_;
}

GSvector3 Actor::velocity() const {
	return velocity_;
}

GSvector3& Actor::velocity() {
	return velocity_;
}

bool Actor::is_grounded() const {
    return is_grounded_;
}

bool& Actor::grounded() {
    return is_grounded_;
}

bool& Actor::use_gravity() {
    return use_gravity_;
}

bool Actor::use_gravity() const {
    return use_gravity_;
}

float Actor::gravity() const {
    return gravity_;
}

float Actor::height() const {
    return height_;
}

void Actor::add_force(const GSvector3& force, ForceMode mode) {
    if (!use_force_external_) return;

    switch (mode) {
    case ForceMode::Force:
        velocity_ += force / mass_;
        break;
    case ForceMode::Impulse:
        velocity_ += force / mass_;
        break;
    case ForceMode::VelocityChange:
        velocity_ += force;
        break;
    }
}

BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

GSmatrix4 Actor::local_to_world(const GSvector3& position, const GSvector3& rotate, const GSvector3& scale) const {
	// 指定されたTranslate, Rotation, Scaleの行列を作成する
	const GSmatrix4 local_matrix = GSmatrix4::TRS(position, GSquaternion::euler(rotate), scale);
	// ワールド空間に変換する
	return local_matrix * transform_.localToWorldMatrix();
}

bool Actor::is_collision() const {
	return enable_collider_;
}

void Actor::message(const std::string& message, std::any& param) {
    // デフォルトは何も受け取らない
}

bool Actor::can_interact() const {
    // デフォルトはできない
	return false;
}

std::string Actor::interact_text() const {
    return "";
}

bool Actor::is_enable_timescale() const {
	return enable_timescale_;
}

bool& Actor::enable_timescale() {
    return enable_timescale_;
}

void Actor::collide_field() {
    // x,z軸の回転を無効にする
    transform_.rotation(GSquaternion(0.0f, transform_.rotation().y, 0.0f, transform_.rotation().w));

    // 壁との衝突判定（球体との判定)
    GSvector3 center; // 押し戻し後の球体の中心座標
    if (world_->get_field()->collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        // 補正後の座標に変更する
        transform_.position(center);
        // ぶつかったら止まる
        velocity_.x = 0.0f;
        velocity_.z = 0.0f;
    }

    // 地面との衝突判定（線分との交差判定)
    // 地面との交点
    GSvector3 intersect;
    // 衝突したフィールド用アクター
    Actor* field_actor{ nullptr };
    // 親をリセットしておく
    transform_.parent(nullptr);

    // 判定座標
    GSvector3 position_head = transform_.position();
    GSvector3 position_foot = transform_.position();
    Line head_line;
    head_line.start = position_head + collider_.center;
    head_line.end = position_head + GSvector3{ 0.0f, height_, 0.0f };
    Line foot_line;
    foot_line.start = position_foot + collider_.center;
    foot_line.end = position_foot + GSvector3{ 0.0f, -FOOT_OFFSET, 0.0f };

    // 天井判定
    if (world_->get_field()->collide(head_line, &intersect, nullptr, &field_actor)) {
        // TODO intersect分yを下げる

        // 座標を変更する
        transform_.position(position_head);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }

    // 地面判定
    if (world_->get_field()->collide(foot_line, &intersect, nullptr, &field_actor)) {
        // 交差した点からy座標のみ補正する
        position_foot.y = intersect.y;
        // 座標を変更する
        transform_.position(position_foot);
        // 重力を初期化する
        velocity_.y = 0.0f;
        // フィールド用のアクタークラスと衝突したか
        if (field_actor != nullptr) {
            // 衝突したフィールド用のアクターを親のトランスフォームクラスとして設定
            transform_.parent(&field_actor->transform());
        }
        // 着地状態の更新
        on_ground();
        is_grounded_ = true;
    }
    else {
        // 空中状態の更新
        on_air();
        is_grounded_ = false;
    }

    // 死亡判定
    if (transform_.position().y < -100.0f) die();
}

void Actor::collide_actor(Actor& other) {
    // 自身と対象の座標を取得
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // 距離を求める
    float distance = GSvector3::distance(position, target);

    // 衝突判定球の半径同士を加えた長さを取得
    float length = collider_.radius + other.collider().radius;

    // 衝突判定の重なっている長さの取得
    float overlap = length - distance;

    // 重なっている部分の半分の距離だけ離れる
    GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
    transform_.translate(v, GStransform::Space::World);

    // フィールドとの衝突判定を再度行う
    collide_field();
}

void Actor::non_penetrating_move(const GSvector3& velocity, GSvector3* foward, float trun_angle) {
	// 移動していたら向きを補間する
	if (foward != nullptr && velocity.magnitude() > 0.01f) {
		// 向きの補間
		GSquaternion rotation = GSquaternion::rotateTowards(
			transform().rotation(),
			GSquaternion::lookRotation(*foward),
			trun_angle
		);
		transform().rotation(rotation);
	}

	// 総移動量
	float total_length = velocity.magnitude();
	// ループ移動時の一度の移動量
	const float translate_length = collider_.radius;
	// 判定する回数
	const int times = ceil(total_length / translate_length);

	if (times > 1) {
		// ループ移動時の移動方向
		const GSvector3 translate_velocity = velocity.normalized() * translate_length;
		for (int i = 0; i < times - 1; ++i) {
			// 移動
			transform().translate(translate_velocity, GStransform::Space::World);
			// 地形との衝突判定
			collide_field();
			// 移動した距離分引く
			total_length -= translate_length;
		}
	}

	// 判定処理に満たない移動量の移動
	const GSvector3 last_velocity = velocity.normalized() * total_length;
	// 移動
	transform().translate(last_velocity, GStransform::Space::World);
	// 地形との衝突判定
	collide_field();
}

void Actor::update_physics(float delta_time) {
    // 重力
    if (use_gravity_ && !is_grounded_) {
        velocity_.y -= gravity_ * delta_time / cFPS;
    }

    if (is_grounded_) {
        // 速度減衰係数
        float damping_factor = std::max(0.0f, 1.0f - (10.0f * delta_time / cFPS));

        velocity_.x *= damping_factor;
        velocity_.z *= damping_factor;
        if (std::abs(velocity_.x) < 0.001f) velocity_.x = 0.0f;
        if (std::abs(velocity_.z) < 0.001f) velocity_.z = 0.0f;
    }

    // 移動量の更新
    if (velocity_.sqrMagnitude() > 0.00001f) {
        GSvector3 move_amount = velocity_ * delta_time;

        // 移動反映
        transform().translate(move_amount, GStransform::Space::World);
    }
}

int Actor::play_effect(GSuint effect_id, const GSvector3& position, const GSvector3& rotate, const GSvector3& scale, float speed) const {
	// エフェクトを再生する
	const GSmatrix4 mat = local_to_world(position, rotate, scale);
    int handle = gsPlayEffectEx(effect_id, &mat);
    gsSetEffectSpeed(handle, speed);
    return handle;
}

void Actor::draw_collider() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glTranslatef(collider_.center.x, collider_.center.y, collider_.center.z);
	glutWireSphere(collider_.radius, 16, 16);
	glPopMatrix();
}
