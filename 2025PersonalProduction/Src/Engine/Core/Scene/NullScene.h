// -----------------------------------------------------------------------------------------
//  File          : NullScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �V�[���}�l�[�W���[�G���[���p��V�[��
//                  ���̃V�[���ɑJ�ڂ��Ă��A�v���P�[�V�����͎����ŏI�����Ȃ�
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef NULL_SCENE_H_
#define NULL_SCENE_H_

#include "Engine/Core/Scene/IScene.h"

class NullScene : public IScene {
public:
    virtual void start() override {}

    virtual void update(float delta_time) override {}

    virtual void draw() const override {}

    virtual void end() override {}

    virtual bool is_end() const override { return false; }

    virtual SceneTag scene_tag() const override {return SceneTag::Null; }

    virtual SceneTag next_scene_tag() const override { return SceneTag::Null; }

    virtual bool is_application_end() const override { return false; }

    virtual void reception_message(const std::string& message, void* param) override {}

};

#endif
