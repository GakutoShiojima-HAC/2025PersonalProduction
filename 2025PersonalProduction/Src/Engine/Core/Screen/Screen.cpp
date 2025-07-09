#include "Engine/Core/Screen/Screen.h"

Screen::~Screen() {
}

Screen& Screen::get_instance() {
	// static�ϐ��̃C���X�^���X�͂P��
	// �C���X�^���X�����P��̂�
	static Screen self;
	return self;
}

void Screen::set_initialize_data(int width_px, int height_px, bool full_screen, float refresh_rate) {
	initialize_data_.width_px = width_px;
	initialize_data_.height_px = height_px;
	initialize_data_.full_screen = full_screen;
	initialize_data_.refresh_rate = refresh_rate;

	current_data_ = initialize_data_;

	// �E�B���h�E�n���h�����擾
	hDWnd = GetActiveWindow();
}

void Screen::update() {
	// �X�N���[���T�C�Y���擾
	GetClientRect(hDWnd, &rect_);
	// �X�N���[���ʒu���擾
	GetWindowRect(hDWnd, &screen_pos_);

	// �X�N���[���f�[�^���X�V
	current_data_.width_px = (int)(rect_.right - rect_.left);
	current_data_.height_px = (int)(rect_.bottom - rect_.top);
}

const ScreenData& Screen::get_initialize_data() const {
	return initialize_data_;
}

ScreenData& Screen::get_current_data() {
	return current_data_;
}


