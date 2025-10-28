// -----------------------------------------------------------------------------------------
//  File          : SceneMenuState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : ���j���[�V�[��
//                  �X�^���_�[�h�V�[��(�X�e�[�g�}�V�[��)�����V�[���ł̂ݎg�p�\
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SCENE_MENU_STATE_H_
#define SCENE_MENU_STATE_H_

#include "State/Scene/SceneState.h"
#include "Engine/Core/Button/ButtonManager.h"

class StandardScene;

class SceneMenuState : public SceneState {
public:
    SceneMenuState(StandardScene& owner);

private:
    void enter() override;

    void update(float delta_time) override;

    void draw() const override;

    void exit() override {};

private:
    StandardScene& owner_;

    ButtonManager button_;

};

#endif
