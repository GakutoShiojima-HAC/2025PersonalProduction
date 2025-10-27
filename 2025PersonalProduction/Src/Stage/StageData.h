#ifndef STAGE_DATA_H_
#define STAGE_DATA_H_

#include <string>

class StageData {
public:
    StageData() = default;

    ~StageData() = default;

public:
    /// <summary>
    /// �X�e�[�W�f�[�^��ǂݍ���
    /// </summary>
    /// <param name="folder_path">= �t�H���_�p�X</param>
    void load(const std::string& folder_path);

public:
    /// <summary>
    /// �X�e�[�W�����擾
    /// </summary>
    /// <returns>�X�e�[�W��(UTF-8)</returns>
    std::string name() const;

    /// <summary>
    /// �t�H���_�����擾
    /// </summary>
    /// <returns></returns>
    std::string folder() const;

private:
    // �X�e�[�W��
    std::string name_{ "unknown" };
    // �f�[�^�̃t�H���_
    std::string folder_{ "" };

public:
    // �R�s�[�֎~
    StageData(const StageData& other) = delete;
    StageData& operator = (const StageData& other) = delete;

};

#endif
