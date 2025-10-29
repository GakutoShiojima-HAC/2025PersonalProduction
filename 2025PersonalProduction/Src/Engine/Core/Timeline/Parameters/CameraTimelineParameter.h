// -----------------------------------------------------------------------------------------
//  File          : CameraTimelineParameter.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/29
//  Updated       : 2025/10/29
//  Description   : �J�����p�^�C�����C���p�����[�^
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_TIMELINE_PARAMETER_H_
#define CAMERA_TIMELINE_PARAMETER_H_

#include "ITimelineParameter.h"
#include <gslib.h>
#include <vector>

class Camera;

/// <summary>
/// �L�[�t���[��
/// </summary>
struct CameraTimelineKeyFrame {
    CameraTimelineKeyFrame(
        float time,
        std::string target,
        GSvector3 position,
        GSvector3 lookat,
        float angle
    ) :
        time{ time },
        target{ target },
        position{ position },
        lookat{ lookat },
        angle{ angle }
    {}

    float time{ 0.0f };						// ����(�b)
    std::string target{ "" };				// ��_�Ƃ���A�N�^�[(���݂��Ȃ���΃��[���h���_����_�Ƃ���)
    GSvector3 position{ 0.0f, 0.0f, 0.0f };	// ��_����̑��΍��W
    GSvector3 lookat{ 0.0f, 0.0f, 0.0f };	// �����_
    float angle{ 0.0f };					// �p�x
};

class CameraTimelineParameter : public ITimelineParameter {
public:
    CameraTimelineParameter(World* world);

    ~CameraTimelineParameter();

public:
    void update(float delta_time) override;

    void clear() override;

    std::string name() const override;

    void load(const std::string& name, const json& j) override;

    void play(const std::string& name) override;

    void stop() override;

private:
    // �^�C�����C���p�J����
    Camera* camera_{ nullptr };
    // �^�C�����C���p�J�������L���ɂȂ�O�̃J����
    Camera* prev_camera_{ nullptr };

#pragma region [�L�[�t���[���Ǘ����s���^�C�����C���f�[�^�N���X]
public:
    /// <summary>
    /// �L�[�t���[���Ǘ����s���^�C�����C���f�[�^�N���X
    /// </summary>
    class CameraTimelineData {
    public:
        CameraTimelineData(const std::vector<CameraTimelineKeyFrame*>& timeline, float start_transition_time = 0.0f, float end_transition_time = 0.0f);

        ~CameraTimelineData();

    public:
        void clear();

    public:
        /// <summary>
        /// �^�C�����C���f�[�^
        /// </summary>
        std::vector<CameraTimelineKeyFrame*>& get();

        /// <summary>
        /// �^�C�����C���p�J�����ɑJ�ڂ���܂ł̎���
        /// </summary>
        float& start_transition_time();

        /// <summary>
        /// ���̃J�����ɑJ�ڂ���܂ł̎���
        /// </summary>
        float& end_transition_time();

    private:
        // �^�C�����C���f�[�^
        std::vector<CameraTimelineKeyFrame*> timeline_;
        // �^�C�����C���p�J�����ɑJ�ڂ���܂ł̎���
        float start_transition_time_{ 0.0f };
        // ���̃J�����ɑJ�ڂ���܂ł̎���
        float end_transition_time_{ 0.0f };

    public:
        // �R�s�[�֎~
        CameraTimelineData(const CameraTimelineData& other) = delete;
        CameraTimelineData& operator = (const CameraTimelineData& other) = delete;

    };
#pragma endregion

public:
    /// <summary>
    /// �^�C�����C���f�[�^���Đ�����
    /// </summary>
    /// <param name="data">= �^�C�����C���f�[�^ ����������͂��܂���</param>
    void play(CameraTimelineData* data);

    /// <summary>
    /// �f�[�^�𐶐�
    /// </summary>
    /// <param name="j">= �f�[�^�I�u�W�F�N�g</param>
    /// <returns>�f�[�^</returns>
    CameraTimelineData* create_data(const json& j);

private:
    /// <summary>
    /// �^�C�����C���f�[�^���Ǘ����ɒǉ�
    /// </summary>
    /// <param name="name">= �Đ����̖��O</param>
    /// <param name="data">= �f�[�^</param>
    void add(const string& name, CameraTimelineData* data);

    /// <summary>
    /// �^�C�����C���f�[�^������
    /// </summary>
    /// <param name="name">= �Đ����̖��O</param>
    CameraTimelineData* find(const std::string& name);

private:
    // �^�C�����C���f�[�^�}�b�v
    std::unordered_map<std::string, CameraTimelineData*> timelines_;
    // ���ݍĐ����̃^�C�����C���f�[�^
    CameraTimelineData* current_{ nullptr };

};

#endif
