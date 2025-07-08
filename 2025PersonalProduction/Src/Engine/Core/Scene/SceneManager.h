// -----------------------------------------------------------------------------------------
//  File          : SceneManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �V�[���Ǘ��N���X
//                  �V���O���g���ɂ��Ă��闝�R�̓V�[���Ԃ̃f�[�^���L��A
//                  �V�[���}�l�[�W���[�𕡐��������ꂽ���Ȃ�����
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <vector>
#include <string>
#include "Scene/SceneTag.h"

using namespace std;

class IScene;

class SceneManager {
private:
    // �R���X�g���N�^�i�O������̃C���X�^���X���֎~�j
    SceneManager() = default;

public:
    ~SceneManager();

public:
    /// <summary>
    /// �C���X�^���X���擾���邽�߂�static�����o�֐�
    /// </summary>
    /// <returns>�C���X�^���X</returns>
    static SceneManager& get_instance();

public:
    /// <summary>
    /// �V�[���}�l�[�W���[�̏�����
    /// </summary>
    void init();

    /// <summary>
    /// ���݂̃V�[�����X�V
    /// </summary>
    /// <param name="delta_time">= �O�t���[������̌o�߃t���[����</param>
    void update(float delta_time);

    /// <summary>
    /// ���݂̃V�[����`��
    /// </summary>
    void draw();

    /// <summary>
    /// ���݂̃V�[�����I��
    /// </summary>
    void end();

    /// <summary>
    /// �o�^�ς݃V�[��������
    /// </summary>
    void clear();

public:
    /// <summary>
    /// �V�[���̒ǉ�
    /// </summary>
    /// <param name="scene">�V�[��</param>
    void add(IScene* scene);

    /// <summary>
    /// �w�肵���V�[���^�O�̃V�[���ɕύX
    /// </summary>
    /// <param name="tag">= �V�[���^�O</param>
    void change(SceneTag tag);

    /// <summary>
    /// �Ǘ����̃V�[����S�ĕԋp
    /// </summary>
    /// <returns>�S�ẴV�[��</returns>
    vector<IScene*> get_scenes() const;

    /// <summary>
    /// �Ǘ����̃V�[���̐���ԋp
    /// </summary>
    /// <returns>�V�[���̐�</returns>
    int count() const;

    /// <summary>
    /// ���݂̃V�[����ԋp
    /// </summary>
    /// <returns>�V�[��</returns>
    IScene* get_current_scene() const;

    /// <summary>
    /// ���݂̃V�[���^�O��ԋp
    /// </summary>
    /// <returns>�V�[���^�O</returns>
    SceneTag get_current_scene_tag() const;

    /// <summary>
    /// �w�肵���V�[��������
    /// </summary>
    /// <param name="tag">= �V�[���^�O</param>
    /// <returns>������΃V�[�����A������Ȃ����nullptr</returns>
    IScene* find(SceneTag tag) const;

    /// <summary>
    /// ���݂̃V�[�����A�v���P�[�V�����I�������N�G�X�g���Ă��邩
    /// </summary>
    /// <returns>���N�G�X�g���Ă�����^</returns>
    bool is_application_end() const;

    /// <summary>
    /// �w�肵���V�[���Ƀ��b�Z�[�W�𑗂�
    /// </summary>
    /// <param name="tag">= ���M��̃V�[���^�O</param>
    /// <param name="message">= ���b�Z�[�W���ʕ�����</param>
    /// <param name="param">= ���b�Z�[�W�p�����[�^</param>
    void send_message(SceneTag tag, const string& message, void* param);

private:
    // �o�^�ς݂̃V�[��
    vector<IScene*> scenes_;
    // ���݂̃V�[��
    IScene* current_scene_{ nullptr };

public:
	// �R�s�[�֎~
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

};

#endif
