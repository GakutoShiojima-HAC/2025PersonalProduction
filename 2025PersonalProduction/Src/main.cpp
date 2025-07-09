// -----------------------------------------------------------------------------------------
//  File          : main.cpp
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/09
//  Description   : �A�v���P�[�V�����Ƃ��ē��삳���邽�߂̋@�\�����N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>
#include "Engine/Core/Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/MenuScene.h"
#include "Scene/GameScene.h"
#include "Engine/Utils/OpenBrowser.h"
#include "Engine/Core/Screen/Screen.h"
#include "Engine/Graphics/Canvas/Canvas.h"

class MyGame : public gslib::Game {
public:
    /// <summary>
    /// �A�v���P�[�V�����R���X�g���N�^
    /// </summary>
    /// <param name="width_px">= �E�B���h�E�̏�����(�s�N�Z��)</param>
    /// <param name="height_px">= �E�B���h�E�̏�������(�s�N�Z��)</param>
    /// <param name="full_screen">= �t���X�N���[���ɂ���Ȃ�^</param>
    /// <param name="refresh_rate">= �A�v���P�[�V�����̃��t���b�V�����[�g</param>
    MyGame(int width_px, int height_px, bool full_screen, float refresh_rate) : gslib::Game{ width_px, height_px, full_screen, refresh_rate } {
        // �X�N���[���f�[�^���Z�b�g
        screen_.set_initialize_data(width_px, height_px, full_screen, refresh_rate);

        // ������
        scene_manager_.init();
        Canvas::init();
    }

private:
    void start() override {
        // �V�[����ǉ�
        scene_manager_.add(new TitleScene{});
        scene_manager_.add(new MenuScene{});
        scene_manager_.add(new GameScene{});

        // �^�C�g���V�[������n�߂�
        scene_manager_.change(SceneTag::Title);
    }

    void update(float delta_time) override {
        screen_.update();

        scene_manager_.update(delta_time);
    }

    void draw() override {
        scene_manager_.draw();
    }

    void end() override {
        scene_manager_.clear();

        // �J�[�\���̈ړ�����������
        ClipCursor(NULL);
        // �J�[�\����\��
        gsShowMouseCursor();

        // �A���P�[�g�t�H�[�����J��
#ifndef _DEBUG
        MyLib::open_browser_by_url("https://forms.gle/RvXri1WB3r3DGJQm7");
#endif
    }

    /// <summary>
    /// �A�v���P�[�V�����̎��s���p�����邩�ǂ���
    /// </summary>
    /// <returns>���s���p������Ȃ�^</returns>
    bool is_running() override {
        // TODO �����I������
        
        // �V�[������̏I�����N�G�X�g
        if (scene_manager_.is_application_end()) return false;

        return true;
    }

private:
    // �V�[���}�l�[�W���[
    SceneManager& scene_manager_ = SceneManager::get_instance();
    // �X�N���[��
    Screen& screen_ = Screen::get_instance();

};

int main() {
#ifdef _DEBUG
    return MyGame(1920, 1080, false, 60.0f).run();
#else
    return MyGame(1920, 1080, true, 60.0f).run();
#endif
}
