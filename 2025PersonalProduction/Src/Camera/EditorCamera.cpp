#include "Camera/EditorCamera.h"
#include <imgui/imgui.h>
#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/Timeline/TimelineManager.h"
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"

// カメラの移動速度
const float MOVE_SPEED{ 0.225f };
// カメラの視点移動速度
const float SENSITIVITY{ 0.075f };

EditorCamera::EditorCamera(IWorld* world) {
	world_ = world;
	tag_ = CameraTag::Editor;

	transform_.position(GSvector3{ 0.0f, 0.0f, 0.0f });
	transform_.lookAt(GSvector3{ 0.0f, 0.0f, 0.0f });

	// カメラタイムラインを取得
	timeline_ = world_->timeline().camera_timeline();

	// もし使用中なら
	if (is_using_) is_active_ = true;
}

void EditorCamera::update(float delta_time) {
	if (is_play_timeline()) return;

#ifdef _DEBUG
	ImGui::Begin("EditorCamera Window");
	string button_text = is_active_ ? "to InActive" : "to Active";
	if (ImGui::Button(button_text.c_str())) toggle_camera();

	if (!is_active_) {
		ImGui::End();
		return;
	};

	ImGui::Text("Right click to move.");

	// 表示用に -180〜180 に変換
	float display_angle = angle_;
	if (display_angle > 180.0f) display_angle -= 360.0f;
	if (ImGui::SliderFloat("Angle", &display_angle, -180.0f, 180.0f)) {
		// 入力値を 0〜360 に再変換する
		angle_ = display_angle;
		if (angle_ < 0.0f) angle_ += 360.0f;
	}
	if (ImGui::Button("Angle Reset")) angle_ = 0.0f;
	
	ImGui::End();

	// 右クリック時だけ操作を受け付ける
	if(input_.action(InputAction::DEBUG_CameraActive)) {
		// カメラ基準の前方向を取得
		GSvector3 forward = transform_.forward();
		forward.y = 0.0f;
		// カメラ基準の右方向を取得
		GSvector3 right = transform_.right();
		right.y = 0.0f;

		// 入力から移動ベクトルを算出
		GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
		GSvector2 input = input_.debug_left_axis();
		velocity += right * input.x;
		velocity += forward * input.y;
		velocity.y += (input_.action(InputAction::DEBUG_Up) ? 1.0f : input_.action(InputAction::DEBUG_Down) ? -1.0f : 0.0f);
		velocity = velocity.normalized() * MOVE_SPEED * delta_time;
		// 移動
		transform_.translate(velocity, GStransform::Space::World);

		// 視点移動
		input = input_.debug_right_axis();
		yaw_ -= input.x * SENSITIVITY;
		pitch_ -= input.y * SENSITIVITY;
		pitch_ = CLAMP(pitch_, -89.0f, 89.0f);
	}
	// 方向
	GSquaternion rotation = GSquaternion::euler(pitch_, yaw_, 0.0f);
	transform_.rotation(rotation);
	// 傾き
	transform_.rotation(transform_.rotation() * GSquaternion::angleAxis(angle_, GSvector3::forward()));
#endif
}

void EditorCamera::exit() {
	if (is_play_timeline() || !is_active_) return;

	// もしほかのカメラに切り替わっても上書きする処理
	is_active_ = true;
	prev_ = world_->get_camera();
	world_->camera_transition(this, 0.0f);
}

void EditorCamera::die() {
	// 外部から死亡判定にできないようにする
}

bool EditorCamera::is_play_timeline() const {
	if (timeline_ == nullptr) return false;
	return timeline_->is_playing();
}

void EditorCamera::toggle_camera() {
	is_active_ = !is_active_;
	if (is_active_) {
		prev_ = world_->get_camera();
		world_->camera_transition(this, 0.0f);
	}
	else {
		world_->camera_transition(prev_, 0.0f);
		prev_ = nullptr;
	}
}
