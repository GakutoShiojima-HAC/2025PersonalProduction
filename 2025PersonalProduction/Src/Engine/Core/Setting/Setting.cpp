#include "Engine/Core/Setting/Setting.h"

Setting& Setting::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static Setting self;
	return self;
}

void Setting::load(const std::string& setting_path) {
	// TODO 今はresetで値を適用しておく
	reset();
}

void Setting::reset() {
	// シーン描画関係
	enable_draw_posteffect_ = true;
	enable_draw_fxaa_ = false;
}

void Setting::save() {
	// TODO 今は無し
}

bool Setting::is_draw_posteffect() const { return enable_draw_posteffect_; }

bool& Setting::enable_draw_posteffect() { return enable_draw_posteffect_; }

bool Setting::is_draw_fxaa() const { return enable_draw_fxaa_; }

bool& Setting::enable_draw_fxaa() { return enable_draw_fxaa_; }
