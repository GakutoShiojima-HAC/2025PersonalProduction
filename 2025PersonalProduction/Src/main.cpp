// -----------------------------------------------------------------------------------------
//  File        : main.cpp
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/08
//  Updated     : 2025/07/08
//  Description : �A�v���P�[�V��������p�N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>

class MyGame : public gslib::Game {
public:
    /// <summary>
    /// �A�v���P�[�V�����R���X�g���N�^
    /// </summary>
    MyGame() : gslib::Game{ 640, 480, false, 60.0f } { }

private:
    /// <summary>
    /// �A�v���P�[�V�����̊J�n
    /// </summary>
    void start() override {

    }

    /// <summary>
    /// �A�v���P�[�V�����̍X�V
    /// </summary>
    void update(float delta_time) override {
    
    }

    /// <summary>
    /// �A�v���P�[�V�����̕`��
    /// </summary>
    void draw() override {
    
    }

    /// <summary>
    /// �A�v���P�[�V�����̏I��
    /// </summary>
    void end() override {
        // �J�[�\���̈ړ�����������
        ClipCursor(NULL);
        // �J�[�\����\��
        gsShowMouseCursor();
    }

    /// <summary>
    /// �A�v���P�[�V�����̋����I������
    /// </summary>
    bool is_running() override {
        // TODO �I������
        
        return true;
    }
};

int main() {
    return MyGame().run();
}
