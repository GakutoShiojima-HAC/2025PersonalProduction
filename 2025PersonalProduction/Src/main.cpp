// -----------------------------------------------------------------------------------------
//  File          : main.cpp
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �A�v���P�[�V�����Ƃ��ē��삳���邽�߂̋@�\�����N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>

class MyGame : public gslib::Game {
public:
    MyGame() : gslib::Game{ 640, 480, false, 60.0f } { }

private:
    void start() override {

    }

    void update(float delta_time) override {
    
    }

    void draw() override {
    
    }

    void end() override {
        // �J�[�\���̈ړ�����������
        ClipCursor(NULL);
        // �J�[�\����\��
        gsShowMouseCursor();
    }

    /// <summary>
    /// �A�v���P�[�V�����̎��s���p�����邩�ǂ���
    /// </summary>
    /// <returns>���s���p������Ȃ�^</returns>
    bool is_running() override {
        // TODO �I������
        
        return true;
    }
};

int main() {
    return MyGame().run();
}
