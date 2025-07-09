// -----------------------------------------------------------------------------------------
//  File          : Screen.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �X�N���[�����Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SCREEN_H_
#define SCREEN_H_

#include <windows.h>

/// <summary>
/// �X�N���[���f�[�^�\����
/// </summary>
struct ScreenData {
    // �E�B���h�E�̕�(�s�N�Z��)
    int width_px{ 0 };
    // �E�B���h�E�̍���(�s�N�Z��)
    int height_px{ 0 };
    // �t���X�N���[�����ǂ���
    bool full_screen{ false };
    // ���t���b�V�����[�g
    float refresh_rate{ 60.0f };
};

class Screen {
private:
    // �R���X�g���N�^�i�O������̃C���X�^���X���֎~�j
    Screen() = default;

public:
    ~Screen();

public:
    /// <summary>
    /// �C���X�^���X���擾���邽�߂�static�����o�֐�
    /// </summary>
    /// <returns>�C���X�^���X</returns>
    static Screen& get_instance();

public:
    /// <summary>
    /// �A�v���P�[�V�����N�����̃X�N���[���f�[�^���Z�b�g����
    /// </summary>
    /// <param name="width_px">= �E�B���h�E�̏�����(�s�N�Z��)</param>
    /// <param name="height_px">= �E�B���h�E�̏�������(�s�N�Z��)</param>
    /// <param name="full_screen">= �t���X�N���[���ɂ���Ȃ�^</param>
    /// <param name="refresh_rate">= �A�v���P�[�V�����̃��t���b�V�����[�g</param>
    void set_initialize_data(int width_px, int height_px, bool full_screen, float refresh_rate);

    /// <summary>
    /// �X�N���[�������X�V
    /// </summary>
    void update();

    /// <summary>
    /// �A�v���P�[�V�����N�����̃X�N���[���f�[�^���擾����
    /// </summary>
    /// <returns>�X�N���[���f�[�^</returns>
    const ScreenData& get_initialize_data() const;

    /// <summary>
    /// ���݂̃X�N���[���f�[�^���擾����
    /// </summary>
    /// <returns>�X�N���[���f�[�^</returns>
    ScreenData& get_current_data();

private:
    // �N�����̃X�N���[���f�[�^
    ScreenData initialize_data_;
    // ���݂̃X�N���[���f�[�^
    ScreenData current_data_;

    // �E�B���h�E�n���h��
    HWND hDWnd{};
    // ���݂̉�ʃT�C�Y
    RECT rect_{};
    // ���݂̉�ʍ��W
    RECT screen_pos_{};

public:
    // �R�s�[�֎~
    Screen(const Screen& other) = delete;
    Screen& operator = (const Screen& other) = delete;

};

#endif
