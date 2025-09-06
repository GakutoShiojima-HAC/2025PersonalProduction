#include "Camera/PlayerCamera.h"
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Utils/Line.h"

// 注視点(基点からのオフセット)
const GSvector3 LOOKAT_ORIGIN_TO_OFFSET{ 0.0f, 1.75f, 0.0f };
// 注視点からの位置
const GSvector3 CAMERA_OFFSET{ 0.0f, 3.0f, -7.0f };
// 視点移動速度
const float SENSITIVITY_X{ 0.095f };
const float SENSITIVITY_Y{ 0.075f };

// スムースダンプ補間時間
const float SMOOTH_TIME{ 2.0f };
// スムースダンプ移動量
const float SMOOTH_MAX_SPEED{ 5.0f };

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
	if (world_->get_field()->collide(line, &intersect)) {
		// 位置を補正(これでも透ける壁は透ける)
		pos = intersect;
	}

	// スムースダンプによる滑らかな補間
	GSvector3 tmp_velocity = GSvector3::zero();	// 仮移動量
	pos = GSvector3::smoothDamp(transform_.position(), pos, tmp_velocity, SMOOTH_TIME, SMOOTH_MAX_SPEED, delta_time);

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
