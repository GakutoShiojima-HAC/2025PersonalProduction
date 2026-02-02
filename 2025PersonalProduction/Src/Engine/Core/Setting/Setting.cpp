#include "Engine/Core/Setting/Setting.h"

Setting::Setting() {
    window_ = new SettingWindow{ *this };
}

Setting::~Setting() {
    delete window_;
    window_ = nullptr;
}

Setting& Setting::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static Setting self;
	return self;
}

void Setting::start() {
    window_->start();
}

void Setting::update(float delta_time) {
    window_->update(delta_time);
}

void Setting::draw() const {
    window_->draw();
}

bool Setting::is_end() const {
    return window_->is_end();
}

void Setting::load(const std::string& setting_path) {
    // 再構築
    delete window_;
    window_ = new SettingWindow{ *this };

    // TODO
}

void Setting::reset() {
	// シーン描画関係
    enable_draw_ssao_ = true;
	enable_draw_bloom_ = true;
	enable_draw_fxaa_ = false;
    // デバッグ関係
    enable_draw_safearea_ = false;
    // デバイス関係
    enable_vibration_ = true;
    sens_ = 1.0f;
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

bool Setting::is_draw_safearea() const { return enable_draw_safearea_; }

bool& Setting::enable_draw_safearea() { return enable_draw_safearea_; }

bool Setting::is_vibration() const { return enable_vibration_; }

bool& Setting::enable_vibration() { return enable_vibration_; }

float Setting::sens_ratio() const {
    return sens_;
}

float& Setting::sens_ratio() {
    return sens_;
}
