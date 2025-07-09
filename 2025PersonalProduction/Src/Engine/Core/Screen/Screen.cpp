#include "Engine/Core/Screen/Screen.h"

Screen::~Screen() {
}

Screen& Screen::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static Screen self;
	return self;
}

void Screen::set_initialize_data(int width_px, int height_px, bool full_screen, float refresh_rate) {
	initialize_data_.width_px = width_px;
	initialize_data_.height_px = height_px;
	initialize_data_.full_screen = full_screen;
	initialize_data_.refresh_rate = refresh_rate;

	// フルスクリーンとリフレッシュレートは不変なのでセット
	current_data_.full_screen = full_screen;
	current_data_.refresh_rate = refresh_rate;
}

const ScreenData& Screen::get_initialize_data() const {
	return initialize_data_;
}

const ScreenData& Screen::get_current_data() const {
	return current_data_;
}


