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
    enable_draw_ssao_ = true;
	enable_draw_bloom_ = true;
	enable_draw_fxaa_ = true;
}

void Setting::save() {
	// TODO 今は無し
}

bool Setting::is_draw_ssao() const { return enable_draw_ssao_; }

bool& Setting::enable_draw_ssao() { return enable_draw_ssao_; }

bool Setting::is_draw_bloom() const { return enable_draw_bloom_; }

bool& Setting::enable_draw_bloom() { return enable_draw_bloom_; }

bool Setting::is_draw_fxaa() const { return enable_draw_fxaa_; }

bool& Setting::enable_draw_fxaa() { return enable_draw_fxaa_; }
