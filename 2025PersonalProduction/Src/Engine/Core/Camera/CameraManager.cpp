#include "Engine/Core/Camera/CameraManager.h"
#include "Engine/Core/Camera/Camera.h"
#include <gslib.h>
#include "GameConfig.h"
#include "Engine/Core/Screen/Screen.h"

CameraManager::CameraManager() {
    screen_data_ = &Screen::get_instance().get_current_data();
}

CameraManager::~CameraManager() {

}

void CameraManager::update(float delta_time) {
	for (const auto& camera : cameras_) {
		camera.second->update(delta_time);
	}

	// 遷移タイマーを更新
	if (prev_ != nullptr && transition_timer_ <= transition_time_) {
		transition_timer_ += delta_time / cFPS;
		// 遷移が終了したら
		if (transition_timer_ >= transition_time_) prev_ = nullptr;
	}

	remove();
    camera_shake_.update(delta_time);
}

void CameraManager::draw() const {
    // 透視射影行列を設定する
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        fov_,				// 視野角
        (float)screen_data_->width_px / (float)screen_data_->height_px,	    // 画面の縦横比（アスペクト比）
        cNEAR,              // 近クリップ面の位置
        cFAR                // 遠クリップ面の位置
    );

	// カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// カメラがなかったら定点描画して終了
	if (current_ == nullptr) {
		draw_empty();
		return;
	}

	// カメラの位置を取得
	GSvector3 pos, at, up;
	camera_lookat(pos, at, up);
	gluLookAt(
		pos.x,	pos.y,	pos.z,
		at.x,	at.y,	at.z,
		up.x,	up.y,	up.z
	);
}

void CameraManager::add(Camera* camera) {
	const GSuint key = (GSuint)camera->tag();

	// 既に存在する場合
	if (cameras_.find(key) != cameras_.end()) {
		if (current_ == camera) current_ = nullptr;
		delete cameras_[key];	// 既存のを消去
		cameras_[key] = camera;
	}
	else {
		cameras_[key] = camera;
	}

	// カメラが存在しない場合
	if (current_ == nullptr) {
		current_ = camera;
		current_->enter();
		current_->is_using() = true;
	}
}

void CameraManager::clear() {
	for (auto& pair : cameras_) {
		delete pair.second;  
	}
	cameras_.clear();
	current_ = nullptr;
	prev_ = nullptr;
}

Camera* CameraManager::current() const {
	return current_;
}

Camera* CameraManager::prev() const {
	return prev_;
}

Camera* CameraManager::find(const CameraTag tag) const {
	auto it = cameras_.find((GSuint)tag);
	if (it != cameras_.end()) {
		return it->second;
	}
	return nullptr;
}

void CameraManager::transition(Camera* to, float time) {
	transition(current_, to, time);
}

void CameraManager::transition(Camera* from, Camera* to, float time) {
	prev_ = from;
	current_ = to;
	if (prev_ != nullptr) {
		prev_->is_using() = false;
		prev_->exit();
	}
	if (current_ != nullptr) {
		current_->is_using() = true;
		current_->enter();
	}
	transition_timer_ = 0.0f;
	transition_time_ = time;

    // 揺れを停止
    camera_shake_.end();
}

float& CameraManager::fov() {
    return fov_;
}

GSmatrix4 CameraManager::get_projection_matrix() const {
    return GSmatrix4::perspective(fov_, (float)screen_data_->width_px / (float)screen_data_->height_px, cNEAR, cFAR);
}

void CameraManager::shake(CameraShakeType type, float duration, float strength, bool loop) {
    camera_shake_.shake(type, duration, strength, loop);
}

void CameraManager::shake_end() {
    camera_shake_.end();
}

bool& CameraManager::enable_shake() {
    return camera_shake_.enable();
}

void CameraManager::remove() {
	for (auto i = cameras_.begin(); i != cameras_.end(); ) {
		Camera* camera = i->second;
		if (camera != nullptr && camera->is_dead()) {
			// もし使っていたらnullptrにする
			if (camera == current_) current_ = nullptr;
			if (camera == prev_) prev_ = nullptr;
			delete camera;
			i = cameras_.erase(i);	// key-valueを削除し、次の要素へ
		}
		else {
			++i;
		}
	}
}

void CameraManager::draw_empty() const {
	gluLookAt(
		0.0f, 0.0f, -10.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);
}

void CameraManager::camera_lookat(GSvector3& pos, GSvector3& at, GSvector3& up) const {
	// トランジションしていない
	if (prev_ == nullptr) {
		pos = current_->transform().transformPoint(camera_shake_.get_offset());
		at = pos + current_->transform().forward();
		up = current_->transform().up();
		return;
	}
	// 進捗率
	const float progress = transition_timer_ / transition_time_;
	// 線形補間する
	pos = GSvector3::lerp(prev_->transform().transformPoint(camera_shake_.get_offset()), current_->transform().transformPoint(camera_shake_.get_offset()), progress);
	at = pos + GSvector3::lerp(prev_->transform().forward(), current_->transform().forward(), progress);
	up = GSvector3::lerp(prev_->transform().up(), current_->transform().up(), progress);
}
