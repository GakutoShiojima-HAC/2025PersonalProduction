// -----------------------------------------------------------------------------------------
//  File          : TitleScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �^�C�g���V�[��
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "Engine/Core/Scene/IScene.h"

class TitleScene : public IScene {
public:
	TitleScene() = default;

	~TitleScene() = default;

public:
	void start() override;
	
	void update(float delta_time) override;

	void draw() const override;

	void end() override;

	bool is_end() const override;

	SceneTag scene_tag() const override;

	SceneTag next_scene_tag() const override;

	bool is_application_end() const override;

	void reception_message(const std::string& message, void* param) override;

private:


private:
	bool is_end_{ false };

};

#endif
