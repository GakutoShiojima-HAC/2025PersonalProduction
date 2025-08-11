#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Utils/Line.h"
#include "GameConfig.h"

void Pawn::take_damage(Actor& other, const int damage) {
	hp_ = CLAMP(hp_ - damage, 0, INT_MAX);
}

void Pawn::on_jump() {
	velocity_.y = jump_power_ * 0.1f + gravity_ * 0.1f / cFPS;	// 重力を加算することで初速を維持
}

int& Pawn::hp() {
	return hp_;
}

float& Pawn::gravity() {
	return gravity_;
}

float& Pawn::invincible_timer() {
	return invincible_timer_;
}

void Pawn::update_gravity(float delta_time) {
	// 重力を加える
	velocity_.y -= gravity_ * 0.1f / cFPS * delta_time;
	// 重力を反映
	transform_.translate(0.0f, velocity_.y, 0.0f);
}

void Pawn::update_invincible(float delta_time) {
	// 衝突判定が有効で無敵時間が無ければ終了
	if (enable_collider_ && invincible_timer_ <= 0.0f) return;

	// 無敵時間を減らす
	invincible_timer_ = CLAMP(invincible_timer_ - delta_time / cFPS, 0.0f, FLT_MAX);
	// 無敵時間が無くなったら衝突判定を復活
	if (invincible_timer_ <= 0.0f && !enable_collider_) enable_collider_ = true;
}

void Pawn::update_mesh(float delta_time) {
	// メッシュのモーションを更新
	mesh_.update(delta_time);
	// ワールド変換行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

void Pawn::collide_field() {
	// x,z軸の回転を無効にする
	transform_.rotation(GSquaternion(0.0f, transform_.rotation().y, 0.0f, transform_.rotation().w));

	/* 壁との衝突判定（球体との判定） */
	GSvector3 center; // 押し戻し後の球体の中心座標
	if (world_->get_field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	
	/* 地面との衝突判定（線分との交差判定）*/
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
	head_line.end = position_head + GSvector3{ 0.0f, head_offset_, 0.0f };
	Line foot_line;
	foot_line.start = position_foot + collider_.center;
	foot_line.end = position_foot + GSvector3{ 0.0f, -foot_offset_, 0.0f };

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
		is_ground_ = true;
	}
	else {
		// 空中状態の更新
		on_air();
		is_ground_ = false;
	}
}

void Pawn::collide_actor(Actor& other) {
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
