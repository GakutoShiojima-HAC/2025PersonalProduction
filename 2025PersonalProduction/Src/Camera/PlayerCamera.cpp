#include "Camera/PlayerCamera.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Utils/Line.h"

// 注視点(基点からのオフセット)
const GSvector3 LOOKAT_ORIGIN_TO_OFFSET{ 0.0f, 1.35f, 0.0f };
// 注視点からの位置
const GSvector3 CAMERA_OFFSET{ 0.0f, 1.3f, -4.0f };
// 視点移動速度
constexpr float SENSITIVITY_X{ 0.3f };
constexpr float SENSITIVITY_Y{ 0.135f };

// スムースダンプ補間時間
constexpr float SMOOTH_TIME{ 6.0f };
// スムースダンプ移動量
constexpr float SMOOTH_MAX_SPEED{ 1.0f };

PlayerCamera::PlayerCamera(IWorld* world) {
	world_ = world;
	tag_ = CameraTag::Player;
}

void PlayerCamera::update(float delta_time) {
	auto exclusion = [](Pawn*& pawn) {
		if (pawn == nullptr) return;
		if (pawn->is_dead_state() || pawn->is_dead()) pawn = nullptr;
	};
	exclusion(owner_);
	exclusion(lockon_target_);

	// カメラ座標の更新
	if (owner_ == nullptr) return;

	GSvector3 pos;
	GSvector3 at;
	if (is_lockon()) {
		// TODO
	}
	else {
		// 視点移動
		GSvector2 input = input_.right_axis();
		if (input_.is_pad()) input *= 15.0f;
		yaw_ -= input.x * SENSITIVITY_X;
		pitch_ -= input.y * SENSITIVITY_Y;
		pitch_ = CLAMP(pitch_, -80.0f, 55.0f);

		// 注視点の座標を求める
		at = owner_->transform().position() + LOOKAT_ORIGIN_TO_OFFSET;
		// カメラの座標を求める
		pos = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * CAMERA_OFFSET;	
	}

	// カメラ座標から注視点間に障害物があるかどうか
	Line line{ at, pos };
	GSvector3 intersect;
    GSplane plane;
	if (world_->get_field()->collide(line, &intersect, &plane)) {
		pos = intersect + plane.normal * 0.1f;  // 障害物から押し返す
	}

	// スムースダンプによる滑らかな補間
	pos = GSvector3::smoothDamp(transform_.position(), pos, vecocity_pos_, SMOOTH_TIME, SMOOTH_MAX_SPEED, delta_time);
    at = GSvector3::smoothDamp(prev_at_, at, vecocity_at_, SMOOTH_TIME, SMOOTH_MAX_SPEED, delta_time);
    prev_at_ = at;

	// 座標の設定
	transform_.position(pos);
	// 注視点の方向を見る
	transform_.lookAt(at);

}

void PlayerCamera::die() {
	// 外部から死亡判定にできないようにする
}

void PlayerCamera::set_owner(Pawn* owner) {
	owner_ = owner;

    if (owner == nullptr) return;

    // 注視点の座標を求める
    GSvector3 at = owner_->transform().position() + LOOKAT_ORIGIN_TO_OFFSET;
    // カメラの座標を求める
    GSvector3 pos = at + owner_->transform().rotation() * CAMERA_OFFSET;
    // 座標の設定
    transform_.position(pos);
    // 注視点の方向を見る
    transform_.lookAt(at);
    prev_at_ = at;

    const GSvector3 direction = at - pos;
    yaw_ = std::atan2f(direction.x, direction.z);
    yaw_ *= (180.0f / GS_PI);
    pitch_ = 0.0f;
}

void PlayerCamera::set_lockon_target(Pawn* target) {
	lockon_target_ = target;
}

bool PlayerCamera::is_lockon() const {
	return lockon_target_ != nullptr;
}

Pawn* PlayerCamera::get_lockon_target() {
	return lockon_target_;
}
