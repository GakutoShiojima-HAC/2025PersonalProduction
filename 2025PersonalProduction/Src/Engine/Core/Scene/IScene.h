// -----------------------------------------------------------------------------------------
//  File          : IScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �V�[���̃C���^�[�t�F�[�X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef I_SCENE_H_
#define I_SCENE_H_

#include <string>
#include "Scene/SceneTag.h"

using namespace std;

class IScene {
public:
    IScene() = default;

    virtual ~IScene() = default;

public:
    /// <summary>
    /// �V�[���J�n
    /// </summary>
    virtual void start() = 0;

    /// <summary>
    /// �X�V
    /// </summary>
    /// <param name="delta_time">= �O�t���[������̌o�߃t���[����</param>
    virtual void update(float delta_time) = 0;

    /// <summary>
    /// �`��
    /// </summary>
    virtual void draw() const = 0;
    
    /// <summary>
    /// �V�[���I��
    /// </summary>
    virtual void end() = 0;

    /// <summary>
    /// �V�[�����I�����Ă��邩
    /// </summary>
    /// <returns>�I�����Ă�����^</returns>
    virtual bool is_end() const = 0;

    /// <summary>
    /// ���݂̃V�[���^�O��ԋp
    /// </summary>
    /// <returns>�V�[���^�O</returns>
    virtual SceneTag scene_tag() const = 0;
    
    /// <summary>
    /// �J�ڐ�̃V�[���^�O��ԋp
    /// </summary>
    /// <returns>�V�[���^�O</returns>
    virtual SceneTag next_scene_tag() const = 0;

    /// <summary>
    /// �V�[�����A�v���P�[�V�����I�������N�G�X�g���Ă��邩
    /// </summary>
    /// <returns>���N�G�X�g���Ă�����^</returns>
    virtual bool is_application_end() const = 0;

    /// <summary>
    /// ���̃V�[�����烁�b�Z�[�W���󂯎��
    /// </summary>
    /// <param name="message">= ���b�Z�[�W���ʕ�����</param>
    /// <param name="param">= ���b�Z�[�W�p�����[�^</param>
    virtual void reception_message(const string& message, void* param) = 0;

public:
    // �R�s�[�֎~
    IScene(const IScene& other) = delete;
    IScene& operator = (const IScene& other) = delete;

};

#endif
