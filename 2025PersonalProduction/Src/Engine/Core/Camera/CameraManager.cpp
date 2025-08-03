#include "Engine/Core/Camera/CameraManager.h"
#include "Engine/Core/Camera/Camera.h"
#include <gslib.h>
#include "GameConfig.h"

CameraManager::~CameraManager() {
	clear();
}

void CameraManager::update(float delta_time) {
	for (const auto& camera : cameras_) {
		camera.second->update(delta_time);
	}

	// �J�ڃ^�C�}�[���X�V
	if (prev_ != nullptr && transition_timer_ <= transition_time_) {
		transition_timer_ += delta_time / cFPS;
		// �J�ڂ��I��������
		if (transition_timer_ >= transition_time_) prev_ = nullptr;
	}

	remove();
}

void CameraManager::draw() const {
	// �J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �J�������Ȃ��������_�`�悵�ďI��
	if (current_ == nullptr) {
		draw_empty();
		return;
	}

	// �J�����̈ʒu���擾
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

	// ���ɑ��݂���ꍇ
	if (cameras_.find(key) != cameras_.end()) {
		if (current_ == camera) current_ = nullptr;
		delete cameras_[key];	// �����̂�����
		cameras_[key] = camera;
	}
	else {
		cameras_[key] = camera;
	}

	// �J���������݂��Ȃ��ꍇ
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
		prev_->exit();
		prev_->is_using() = false;
	}
	if (current_ != nullptr) {
		current_->enter();
		current_->is_using() = true;
	}
	transition_timer_ = 0.0f;
	transition_time_ = time;
}

void CameraManager::remove() {
	for (auto i = cameras_.begin(); i != cameras_.end(); ) {
		Camera* camera = i->second;
		if (camera != nullptr && camera->is_dead()) {
			// �����g���Ă�����nullptr�ɂ���
			if (camera == current_) current_ = nullptr;
			if (camera == prev_) prev_ = nullptr;
			delete camera;
			i = cameras_.erase(i);	// key-value���폜���A���̗v�f��
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
	// �g�����W�V�������Ă��Ȃ�
	if (prev_ == nullptr) {
		pos = current_->transform().position();
		at = pos + current_->transform().forward();
		up = current_->transform().up();
		return;
	}
	// �i����
	const float progress = transition_timer_ / transition_time_;
	// ���`��Ԃ���
	pos = GSvector3::lerp(prev_->transform().position(), current_->transform().position(), progress);
	at = pos + GSvector3::lerp(prev_->transform().forward(), current_->transform().forward(), progress);
	up = GSvector3::lerp(prev_->transform().up(), current_->transform().up(), progress);
}
