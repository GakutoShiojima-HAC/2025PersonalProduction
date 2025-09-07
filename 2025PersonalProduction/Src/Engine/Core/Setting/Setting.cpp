#include "Engine/Core/Setting/Setting.h"

Setting& Setting::get_instance() {
	// static�ϐ��̃C���X�^���X�͂P��
	// �C���X�^���X�����P��̂�
	static Setting self;
	return self;
}

void Setting::load(const std::string& setting_path) {
	// TODO ����reset�Œl��K�p���Ă���
	reset();
}

void Setting::reset() {
	// �V�[���`��֌W
	enable_draw_posteffect_ = true;
	enable_draw_fxaa_ = false;
}

void Setting::save() {
	// TODO ���͖���
}

bool Setting::is_draw_posteffect() const { return enable_draw_posteffect_; }

bool& Setting::enable_draw_posteffect() { return enable_draw_posteffect_; }

bool Setting::is_draw_fxaa() const { return enable_draw_fxaa_; }

bool& Setting::enable_draw_fxaa() { return enable_draw_fxaa_; }
